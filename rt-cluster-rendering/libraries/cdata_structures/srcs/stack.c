/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merras <merras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 21:44:41 by merras            #+#    #+#             */
/*   Updated: 2020/02/29 21:57:38 by merras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stack.h"

void	stack_push(t_dstruct_list *stack, t_dstruct_node *node)
{
	if (!stack || !node)
		return ;
	if (!stack->head)
	{
		node->next = NULL;
		node->prev = NULL;
		stack->head = node;
		stack->tail = node;
	}
	else
	{
		node->next = NULL;
		node->prev = stack->tail;
		stack->tail->next = node;
		stack->tail = node;
	}
	stack->size++;
}

void	stack_pop(t_dstruct_list *stack, void (*node_deconstructor)(void *))
{
	t_dstruct_node *popped;

	if (!stack || !stack->tail)
		return ;
	popped = stack->tail;
	stack->tail = stack->tail->prev;
	if (stack->tail)
		stack->tail->next = NULL;
	else
		stack->head = NULL;
	if (node_deconstructor)
		node_deconstructor(popped->content);
	free(popped);
	stack->size--;
}
