#include "lists_wrappers.h"

t_dstruct_list  t_dstruct_list_init(void)
{
    t_dstruct_list  list;

    list.head = NULL;
    list.tail = NULL;
    list.size = 0;
    return (list);
}

t_dstruct_node  *t_dstruct_create_node(void *content, size_t content_size)
{
    t_dstruct_node *node;

    if (!(node = malloc(sizeof(t_dstruct_node))))
        return (NULL);
    node->content = content;
    node->content_size = content_size;
    node->next = NULL;
    node->prev = NULL;
    return (node);
}