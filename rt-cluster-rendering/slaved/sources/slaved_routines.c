/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   slaved_routines.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merras <merras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/01 20:06:36 by merras            #+#    #+#             */
/*   Updated: 2020/03/03 15:32:54 by merras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "slaved.h"

static void	handle_packet(void *slaved)
{
	t_packet		response;
	t_dstruct_node	*request;

	request = CAST(slaved, t_slaved *)->packets_queue.head;
	queue_dequeue(&CAST(slaved, t_slaved *)->packets_queue, NULL);
	if (pthread_mutex_unlock(&CAST(slaved, t_slaved *)->packets_queue_mx))
		pthread_exit(NULL);
	response = requests_dispatcher(request->content, slaved);
	if (write_packet(CAST(slaved, t_slaved *)->sockets[1], response) ||
		response.type == TYPE_T_RESPONSE_FAILURE ||
		CAST(request->content, t_packet *)->type == TYPE_T_REQUEST_COMPUTATION)
		pthread_cancel(CAST(slaved, t_slaved *)->tid[0]);
	destroy_packet(request->content);
	free(request);
}

void		*handle_packets(void *slaved)
{
	while (1)
	{
		pthread_testcancel();
		if (pthread_mutex_lock(&CAST(slaved, t_slaved *)->packets_queue_mx))
			pthread_exit(NULL);
		if (!CAST(slaved, t_slaved *)->packets_queue.size)
		{
			if (pthread_mutex_unlock(
				&CAST(slaved, t_slaved *)->packets_queue_mx))
				pthread_exit(NULL);
			continue;
		}
		else
			handle_packet(slaved);
	}
}

void		*receive_packets(void *slaved)
{
	t_packet request;

	LOG_INFO("running receive_packets() routine.");
	while (1)
	{
		LOG_INFO("reading a request.");
		if (read_packet(CAST(slaved, t_slaved *)->sockets[1], &request))
		{
			LOG_ERROR("exiting receive_packets(), read_packet() failed()");
			pthread_exit(NULL);
		}
		if (pthread_mutex_lock(&CAST(slaved, t_slaved *)->packets_queue_mx))
			pthread_exit(NULL);
		LOG_INFO("enquing a request.");
		queue_enqueue(&CAST(slaved, t_slaved *)->packets_queue,
			t_dstruct_create_node(create_packet(request, NULL),
				sizeof(t_packet)));
		if (pthread_mutex_unlock(&CAST(slaved, t_slaved *)->packets_queue_mx))
			pthread_exit(NULL);
		if (request.type == TYPE_T_REQUEST_COMPUTATION)
			LOG_WARN("receive_packets: enqueing a COMPUTATION, I am done.");
	}
}
