#include "../inc/libmx.h"

void mx_push_front(t_list **list, void *data)
{
    t_list *new_list = mx_create_node(data);
    new_list->next = *list;

    *list = new_list;
}
