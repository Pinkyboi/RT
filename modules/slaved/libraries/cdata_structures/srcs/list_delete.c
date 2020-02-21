#include "lists_wrappers.h"

void	list_delete_node(t_dstruct_list *list, t_dstruct_node *node,
			void (*node_deconstructor)(void *))
{
	t_dstruct_node *e;

	if (!e || !node)
		return ;
	e = list->head;
	while (e)
	{
		if (e == node)
		{
			if (e->next)
				e->next->prev = e->prev;
			if (!e->prev && !(list->head = NULL))
				list->tail = NULL;
			else
				e->prev->next = e->next;
			if (node_deconstructor)
			{
				node_deconstructor(e->content);
				free(e);
			}
			list->size--;
		}
		e = e->next;
	}
}
