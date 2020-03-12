/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merras <merras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 21:45:21 by merras            #+#    #+#             */
/*   Updated: 2020/02/29 22:05:28 by merras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STACK_H
# define STACK_H

# include "lists_wrappers.h"

void	stack_push(t_dstruct_list *stack, t_dstruct_node *node);
void	stack_pop(t_dstruct_list *stack, void (*node_deconstructor)(void *));

#endif
