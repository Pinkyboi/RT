/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merras <merras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 21:44:38 by merras            #+#    #+#             */
/*   Updated: 2020/02/29 21:56:50 by merras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "queue.h"

void	queue_enqueue(t_dstruct_list *queue, t_dstruct_node *node)
{
	stack_push(queue, node);
}

void	queue_dequeue(t_dstruct_list *queue, void (*node_deconstructor)(void *))
{
	t_dstruct_node *deqeued;

	if (!queue || !queue->tail)
		return ;
	deqeued = queue->head;
	queue->head = queue->head->next;
	if (queue->head)
		queue->head->prev = NULL;
	else
		queue->tail = NULL;
	if (node_deconstructor)
	{
		node_deconstructor(deqeued->content);
		free(deqeued);
	}
	queue->size--;
}
