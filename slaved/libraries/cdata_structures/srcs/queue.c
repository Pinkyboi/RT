#include "queue.h"

void    queue_enqueue(t_dstruct_list *queue, t_dstruct_node *node)
{
    stack_push(queue, node);
}

void    queue_dequeue(t_dstruct_list *queue, void (* node_deconstructor)(void *))
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