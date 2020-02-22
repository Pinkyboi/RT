#include "slaved.h"

void     *handle_packets(void *slaved)
{
    t_packet response;
    t_packet *request;

    while (1)
    {
        pthread_mutex_lock(&CAST(slaved, t_slaved *)->packets_queue_mx);
        if (!CAST(slaved, t_slaved *)->packets_queue.size)
        {
            pthread_mutex_unlock(&CAST(slaved, t_slaved *)->packets_queue_mx);
            continue ;
        }
        else
        {
            request = CAST(slaved, t_slaved *)->packets_queue.head->content;
            queue_dequeue(&CAST(slaved, t_slaved *)->packets_queue, NULL);
            pthread_mutex_unlock(&CAST(slaved, t_slaved *)->packets_queue_mx);
            response = requests_dispatcher(request, CAST(slaved, t_slaved *)->computation, (int[2]){CAST(slaved, t_slaved *)->server_socket,
				CAST(slaved, t_slaved *)->connection_socket});
            destroy_packet(request);
            if (request->type == TYPE_T_REQUEST_FINISH)
                pthread_exit(NULL);
            if (write_packet(CAST(slaved, t_slaved *)->connection_socket, response) ||
                response.type == TYPE_T_RESPONSE_FAILURE)
            {
                pthread_cancel(CAST(slaved, t_slaved *)->tid[0]);
                pthread_exit(NULL);
			}
			sleep(2);
        }
    }
}

void    *receive_packets(void *slaved)
{
    t_packet request;

	delete = slaved;
	log_info("running receive_packets() routine.");
    while (1)
    {
		log_info("reading a request.");
        if (read_packet(CAST(slaved, t_slaved *)->connection_socket, &request))
		{
			log_warn("exiting receive_packets() routine, read_packet() failed.");
            pthread_exit(NULL);
		}
        pthread_mutex_lock(&CAST(slaved, t_slaved *)->packets_queue_mx);
		log_info("enquing a request.");
        queue_enqueue(&CAST(slaved, t_slaved *)->packets_queue,
            t_dstruct_create_node(create_packet(request, NULL), sizeof(t_packet)));
        pthread_mutex_unlock(&CAST(slaved, t_slaved *)->packets_queue_mx);
        if (request.type == TYPE_T_REQUEST_FINISH)
		{
			log_warn("exiting receive_packets() routine, received FINISH_PACKET_TYPE.");
            pthread_exit(NULL);
		}
    }
}

int connect_master(t_slaved *slaved)
{
    struct sockaddr_in  slave_address;
    struct sockaddr_in  master_address;
    socklen_t           address_length;
    int                 err;

    err = 0;
    if ((slaved->server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
        return (1);
	}
    ft_bzero(&slave_address, sizeof(slave_address));
    ft_bzero(&master_address, sizeof(master_address));
    slave_address.sin_family = AF_INET;
    slave_address.sin_addr.s_addr = htonl(INADDR_ANY);
    slave_address.sin_port = htons(PORT);
    if (bind(slaved->server_socket, (struct sockaddr *)&slave_address,
        sizeof(slave_address)) ||
        listen(CAST(slaved, t_slaved *)->server_socket, 1))
        return (2);
    while (1)
    {
        if ((slaved->connection_socket =
            accept(slaved->server_socket, (struct sockaddr *)
            &master_address, &address_length)) == -1)
            return (3);
		log_info("accepting a master connection.");
        err = ERROR_WRAPPER(pthread_create(&slaved->tid[0], NULL, receive_packets, slaved));
        err = ERROR_WRAPPER(pthread_create(&slaved->tid[1], NULL, handle_packets, slaved));
		log_debug("server socket: %p %d.", slaved, slaved->server_socket);
        if (err)
            return (4);
        if (!pthread_join(slaved->tid[0], NULL))
        {
			log_info("\treceive_packets() routine joined, cancelling handle_packets() routine.");
            pthread_cancel(slaved->tid[1]);
			log_info("\tcleaning up resources.");
            cleanup(slaved->computation);
            list_delete(&slaved->packets_queue, destroy_packet);
        }
		log_info("disconnecting a master.");
        close(CAST(slaved, t_slaved *)->connection_socket);
    }
}

#include <string.h>
#include <errno.h>

int main(void)
{
    t_slaved    slaved;
	int			err;

    slaved.packets_queue = t_dstruct_list_init();
    slaved.computation = init_computation();
    pthread_mutex_init(&slaved.packets_queue_mx, NULL);

    if ((err = connect_master(&slaved)))
		log_fatal("connect_master() failed: %d %s.", err, strerror(errno));
}