#ifndef STACK_H
# define STACK_H

# include "lists_wrappers.h"

void    stack_push(t_dstruct_list *stack, t_dstruct_node *node);
void    stack_pop(t_dstruct_list *stack, void (* node_deconstructor)(void *));

#endif