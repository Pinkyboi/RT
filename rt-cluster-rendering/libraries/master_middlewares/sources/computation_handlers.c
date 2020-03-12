/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   computation_handlers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merras <merras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/02 06:29:14 by merras            #+#    #+#             */
/*   Updated: 2020/03/04 19:00:02 by merras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "master_middlewares.h"

t_task	*create_task(t_task value)
{
	t_task	*task;

	if (!(task = malloc(sizeof(t_task))))
		return (NULL);
	task->request = value.request;
	task->response = value.response;
	return (task);
}

int		init_computation(t_cluster *cluster)
{
	pthread_mutex_init(&cluster->computation.tasks_queue_mutex, NULL);
	pthread_mutex_init(&cluster->computation.done_queue_mutex, NULL);
	cluster->computation.tasks_queue = t_dstruct_list_init();
	cluster->computation.done_queue = t_dstruct_list_init();
	return (0);
}

void	queue_task(t_cluster *cluster, t_packet *request)
{
	pthread_mutex_lock(&cluster->computation.tasks_queue_mutex);
	queue_enqueue(&cluster->computation.tasks_queue,
		t_dstruct_create_node(create_task((t_task){request, NULL}),
		sizeof(t_task)));
	cluster->computation.size++;
	pthread_mutex_unlock(&cluster->computation.tasks_queue_mutex);
}

int		fetch_computation_blob_wait(t_cluster *cluster)
{
	while (1)
	{
		pthread_mutex_lock(&cluster->computation.done_queue_mutex);
		if (cluster->computation.done_queue.size == cluster->size)
			return (0);
		if (cluster->error)
		{
			pthread_cancel(cluster->loadbalancer_routine);
			pthread_join(cluster->loadbalancer_routine, NULL);
			pthread_mutex_unlock(&cluster->computation.done_queue_mutex);
			return (1);
		}
		pthread_mutex_unlock(&cluster->computation.done_queue_mutex);
	}
}

void	**fetch_computation_blob(t_cluster *cluster,
			int (*compare)(void *, void *))
{
	t_dstruct_node	*done_task;
	void			**blob;
	int				i;

	(void)compare;
	if (fetch_computation_blob_wait(cluster))
		return (NULL);
	if (compare)
		list_sort(&cluster->computation.done_queue, compare);
	done_task = cluster->computation.done_queue.head;
	if (!(blob = (void **)malloc(sizeof(void *) *
		(cluster->computation.done_queue.size + 1))))
		return (NULL);
	i = 0;
	while (done_task)
	{
		blob[i] = done_task->content;
		done_task = done_task->next;
		i++;
	}
	blob[i] = NULL;
	pthread_mutex_unlock(&cluster->computation.done_queue_mutex);
	return (blob);
}
