/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cluster_loadbalancer.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merras <merras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/02 05:24:44 by merras            #+#    #+#             */
/*   Updated: 2020/03/04 15:44:52 by merras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "master_middlewares.h"

void	enqueue_task_to_all_slaves(t_dstruct_node *task, t_cluster *cluster)
{
	int i;

	i = -1;
	while (++i < (int)CAST(cluster, t_cluster *)->size)
	{
		pthread_mutex_lock(&CAST(cluster, t_cluster *)->nodes[i].mutex);
		queue_enqueue(&CAST(cluster, t_cluster *)->nodes[i].tasks_queue,
			t_dstruct_create_node(task->content, task->content_size));
		pthread_mutex_unlock(&CAST(cluster, t_cluster *)->nodes[i].mutex);
	}
}

void	enqueue_task_to_offset_slaves(t_dstruct_node *task, t_cluster *cluster)
{
	if (CAST(cluster, t_cluster *)->offset >=
		(int)CAST(cluster, t_cluster *)->size)
		return ;
	pthread_mutex_lock(&CAST(cluster, t_cluster *)->nodes[
		CAST(cluster, t_cluster *)->offset].mutex);
	queue_enqueue(&CAST(cluster, t_cluster *)->nodes[
		CAST(cluster, t_cluster *)->offset].tasks_queue,
		t_dstruct_create_node(task->content, task->content_size));
	pthread_mutex_unlock(&CAST(cluster, t_cluster *)->nodes[
		CAST(cluster, t_cluster *)->offset].mutex);
	CAST(cluster, t_cluster *)->offset++;
}

void	cluster_loadbalancer_wait(void *cluster)
{
	while (1)
	{
		pthread_testcancel();
		pthread_mutex_lock(&CAST(cluster, t_cluster *)->
			computation.tasks_queue_mutex);
		if (!CAST(cluster, t_cluster *)->computation.tasks_queue.head)
		{
			pthread_mutex_unlock(&CAST(cluster, t_cluster *)->
				computation.tasks_queue_mutex);
			continue ;
		}
		return ;
	}
}

void	*cluster_loadbalancer(void *cluster)
{
	t_dstruct_node *task;

	LOG_INFO("running loadbalancer routine.");
	while (1)
	{
		cluster_loadbalancer_wait(cluster);
		pthread_testcancel();
		LOG_INFO("receiving a workload.");
		task = CAST(cluster, t_cluster *)->computation.tasks_queue.head;
		queue_dequeue(&CAST(cluster, t_cluster *)->computation.tasks_queue,
			NULL);
		pthread_mutex_unlock(&CAST(cluster, t_cluster *)->
			computation.tasks_queue_mutex);
		if (CAST(task->content, t_task *)->request->type !=
			TYPE_T_REQUEST_COMPUTATION)
			enqueue_task_to_all_slaves(task, cluster);
		else
			enqueue_task_to_offset_slaves(task, cluster);
		free(task);
	}
}
