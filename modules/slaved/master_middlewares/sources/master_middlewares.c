#include "master_middlewares.h"

int init_slaves(t_cluster *cluster)
{
    t_packet    request;
    int         err;

	log_info("initializing slaves, sending INIT, TEXTURES, SCENE requests.");
    err = 0;
    request.type = TYPE_T_REQUEST_INIT;
    err = ERROR_WRAPPER((request.data = read_file(cluster->program, &request.size)) == NULL);
    if (!err)
        queue_task(cluster, create_packet(request, NULL));
    return (err);
}

/*
** slave_routine()
**  - loop over tasks queue and send requests.
**  - wait for response:
**      - if successful, update task accordingly.
**      - if failed, enqueue task again to the global task queue.
*/

void    *slave_routine(void *slave)
{
    t_dstruct_node *task;
    t_packet        response;
    int             err;

    err = 0;

	log_info("running slave routine.");
    while (1)
    {
        pthread_mutex_lock(&CAST(slave, t_slave *)->mutex);
        if (!CAST(slave, t_slave *)->tasks_queue.head)
        {
            pthread_mutex_unlock(&CAST(slave, t_slave *)->mutex);
            continue ;
        }
        task = CAST(slave, t_slave *)->tasks_queue.head;
        queue_dequeue(&CAST(slave, t_slave *)->tasks_queue, NULL);
        pthread_mutex_unlock(&CAST(slave, t_slave *)->mutex);
        if (write_packet(CAST(slave, t_slave *)->socket, *(CAST(task->content, t_task *)->request)))
        {
			log_warn("error writing request, exiting slave_routine().");
            pthread_exit(NULL);
        }
        if (read_packet(CAST(slave, t_slave *)->socket, &response))
        {
			log_warn("error reading response, exiting slave_routine().");
            pthread_exit(NULL);
        }
        CAST(task->content, t_task *)->response = create_packet(response, NULL);
        if (CAST(task->content, t_task *)->response->type == TYPE_T_RESPONSE_SUCCESS)
        {
			if (CAST(task->content, t_task *)->request->type == TYPE_T_REQUEST_COMPUTATION)
			{
				log_debug("enquing a request to done queue, %d.",
					CAST(slave, t_slave *)->cluster->computation.done_queue.size);
        		pthread_mutex_lock(&CAST(slave, t_slave *)->cluster->computation.done_queue_mutex);
            	queue_enqueue(&(CAST(slave, t_slave *)->cluster->computation.done_queue), task);
        		pthread_mutex_unlock(&CAST(slave, t_slave *)->cluster->computation.done_queue_mutex);
			}
			log_debug("successful task execution.");
        }
        else
        {
			log_debug("failed task execution.");
            //free failure response
        	pthread_mutex_lock(&CAST(slave, t_slave *)->cluster->computation.tasks_queue_mutex);
            queue_enqueue(&(CAST(slave, t_slave *)->cluster->computation.tasks_queue), task);
        	pthread_mutex_lock(&CAST(slave, t_slave *)->cluster->computation.tasks_queue_mutex);
        }
        pthread_mutex_lock(&CAST(slave, t_slave *)->cluster->least_used_slave->mutex);
        if (CAST(slave, t_slave *)->tasks_queue.size <
            CAST(slave, t_slave *)->cluster->least_used_slave->tasks_queue.size)
            CAST(slave, t_slave *)->cluster->least_used_slave = slave;
        pthread_mutex_unlock(&CAST(slave, t_slave *)->cluster->least_used_slave->mutex);
    }
}

/*
** connect_slaves()
**  - iterate over cluster slaves, and lunch a thread for every slave.
*/

#include <errno.h>

int connect_slaves(t_cluster *cluster)
{
    int slave_socket;
    struct sockaddr_in  slave;
    int i;
    int err;
    pthread_t tid;

    err = 0;
    i = -1;

	log_info("connecting slaves.");
    while (++i < cluster->size)
    {
        err = ERROR_WRAPPER((slave_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1);
        ft_bzero(&slave, sizeof(slave));

        slave.sin_family = AF_INET;
        slave.sin_addr.s_addr = inet_addr(cluster->nodes[i].ip);
        slave.sin_port = htons(PORT);
        err = ERROR_WRAPPER(connect(slave_socket, (struct sockaddr *)&slave, sizeof(slave)) != 0);
        cluster->nodes[i].socket = err ? -1 : slave_socket;
        if (err)
        {
			log_error("error connecting a slave.");
            continue ;
        }
        err = ERROR_WRAPPER(pthread_create(&tid, NULL, slave_routine, &(cluster->nodes[i])));
    }
    return (0);
}

/*
** cluster_loadbalancer()
**
*/

void    *cluster_loadbalancer(void *cluster)
{
    t_dstruct_node    *task;
    int         i;

	log_info("running loadbalancer routine.");
    while (1)
    {
        pthread_mutex_lock(&CAST(cluster, t_cluster *)->computation.tasks_queue_mutex);
        if (!CAST(cluster, t_cluster *)->computation.tasks_queue.head)
        {
            pthread_mutex_unlock(&CAST(cluster, t_cluster *)->computation.tasks_queue_mutex);
            continue ;
        }
		log_info("receiving a workload.");
        task = CAST(cluster, t_cluster *)->computation.tasks_queue.head;
        queue_dequeue(&CAST(cluster, t_cluster *)->computation.tasks_queue, NULL);
        pthread_mutex_unlock(&CAST(cluster, t_cluster *)->computation.tasks_queue_mutex);
        if (CAST(task->content, t_task *)->request->type != TYPE_T_REQUEST_COMPUTATION)
        {
            i = -1;
            while (++i < CAST(cluster, t_cluster *)->size)
            {
                pthread_mutex_lock(&CAST(cluster, t_cluster *)->nodes[i].mutex);
                queue_enqueue(&CAST(cluster, t_cluster *)->nodes[i].tasks_queue, task);
                pthread_mutex_unlock(&CAST(cluster, t_cluster *)->nodes[i].mutex);
            }
        }
        else
        {
            pthread_mutex_lock(&CAST(cluster, t_cluster *)->least_used_slave->mutex);
            queue_enqueue(&CAST(cluster, t_cluster *)->least_used_slave->tasks_queue, task);
            pthread_mutex_unlock(&CAST(cluster, t_cluster *)->least_used_slave->mutex);
        }
    }
}

/* init_cluster()
**  - parsing configuration file.
**  - initaiting connection with slaves.
*/

int init_cluster(char *configuration_file, t_cluster *cluster)
{
    int err;
    pthread_t   tid;

	log_info("initiating cluster.");
    err = 0;
    err = ERROR_WRAPPER(get_configuration(configuration_file, cluster) != 0);
    if (!cluster->size)
        return (1);
    cluster->least_used_slave = &cluster->nodes[0];
    err = ERROR_WRAPPER(connect_slaves(cluster) != 0);
    if (!err)
        init_computation(cluster);
    err = ERROR_WRAPPER(init_slaves(cluster) != 0);
    err = ERROR_WRAPPER(pthread_create(&tid, NULL, cluster_loadbalancer, cluster) != 0);
    return (err);
}

