#ifndef QUEUE_H
# define QUEUE_H

# include "lists_wrappers.h"
# include "stack.h"

void    queue_enqueue(t_dstruct_list *queue, t_dstruct_node *node);
void    queue_dequeue(t_dstruct_list *queue, void (* node_deconstructor)(void *));

#endif