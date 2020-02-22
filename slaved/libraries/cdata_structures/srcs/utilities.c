#include "cdata_structures.h"

size_t	list_counter(t_dstruct_list list, int (*filter)(void *))
{
	size_t	count;

	count = 0;
	while (list.head)
	{
		count += filter(list.head->content);
		list.head = list.head->next;
	}
	return (count);
}


t_dstruct_node *list_find_node(t_dstruct_list list, int (*filter)(void *, void *), void *argument)
{
	while (list.head)
	{
		if (filter && filter(list.head->content, argument))
			return (list.head);
		list.head = list.head->next;
	}
	return (NULL);
}

t_dstruct_list	list_copy(t_dstruct_list list, int (*filter)(void *))
{
	t_dstruct_list copy;
	t_dstruct_node *node;
	t_dstruct_node *node_copy;

	copy = t_dstruct_list_init();
	node = list.head;
	while (node)
	{
		if (filter && filter(node->content))
		{
			node_copy = malloc(sizeof(t_dstruct_node));
			ft_memcpy(node_copy, node, sizeof(t_dstruct_node));
			queue_enqueue(&copy, node_copy);
		}
		node = node->next;
	}
	return (copy);
}

void	list_delete(t_dstruct_list *list, void (*deconstructor)(void *))
{
	while (list->size)
		list_delete_node(list, list->tail, deconstructor);
}

int		list_node_index(t_dstruct_list list, t_dstruct_node *node)
{
	int i;

	i = 0;
	while (list.head != node)
	{
		list.head = list.head->next;
		i++;
	}
	return (i);
}
