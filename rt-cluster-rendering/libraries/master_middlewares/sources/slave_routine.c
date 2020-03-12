/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   slave_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merras <merras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/02 09:09:11 by merras            #+#    #+#             */
/*   Updated: 2020/03/04 15:44:51 by merras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "master_middlewares.h"

void	slave_wait(t_slave *slave)
{
	while (1)
	{
		pthread_testcancel();
		pthread_mutex_lock(&slave->mutex);
		if (!slave->tasks_queue.head)
		{
			pthread_mutex_unlock(&slave->mutex);
			continue;
		}
		return ;
	}
}

void	slave_write_dolan(t_cluster *cluster, void *caller)
{
	LOG_WARN("error writing request, exiting slave_routine().");
	cancel_routines(cluster, caller);
	cluster->error = 1;
	pthread_exit(NULL);
}

void	slave_read_dolan(t_cluster *cluster, void *caller)
{
	LOG_WARN("error reading response, exiting slave_routine().");
	cancel_routines(cluster, caller);
	cluster->error = 1;
	pthread_exit(NULL);
}

void	success_response_dolan(t_cluster *cluster, t_dstruct_node *task)
{
	if (CAST(task->content, t_task *)->request->type ==
		TYPE_T_REQUEST_COMPUTATION)
	{
		LOG_DEBUG("enquing a request to done queue.");
		pthread_mutex_lock(&cluster->computation.done_queue_mutex);
		queue_enqueue(&cluster->computation.done_queue,
			t_dstruct_create_node(task->content, task->content_size));
		pthread_mutex_unlock(&cluster->computation.done_queue_mutex);
		free(task);
		pthread_exit(NULL);
	}
}

void	*slave_routine(void *slave)
{
	t_dstruct_node	*task;
	t_packet		response;

	while (1)
	{
		slave_wait(slave);
		task = CAST(slave, t_slave *)->tasks_queue.head;
		queue_dequeue(&CAST(slave, t_slave *)->tasks_queue, NULL);
		pthread_mutex_unlock(&CAST(slave, t_slave *)->mutex);
		if (write_packet(CAST(slave, t_slave *)->socket,
			*CAST(task->content, t_task *)->request))
			slave_write_dolan(CAST(slave, t_slave *)->cluster, slave);
		if (read_packet(CAST(slave, t_slave *)->socket, &response))
			slave_read_dolan(CAST(slave, t_slave *)->cluster, slave);
		CAST(task->content, t_task *)->response = create_packet(response, NULL);
		if (response.type == TYPE_T_RESPONSE_SUCCESS)
			success_response_dolan(CAST(slave, t_slave *)->cluster, task);
		else
		{
			LOG_DEBUG("failed task execution.");
			cancel_routines(CAST(slave, t_slave *)->cluster, slave);
			CAST(CAST(slave, t_slave *)->cluster, t_cluster *)->error = 1;
			pthread_exit(NULL);
		}
	}
}
