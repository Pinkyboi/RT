/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/26 07:22:58 by abenaiss          #+#    #+#             */
/*   Updated: 2020/01/01 02:45:10 by abenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_object_list	*copy_objects(t_object_list *head)
{
	t_object_list *objects;
	t_object_list *new_list;
	t_object_list *last_object;

	objects = head;
	new_list = NULL;
	last_object = NULL;
	if (!(new_list = malloc(sizeof(t_object_list))))
		return (NULL);
	last_object = new_list;
	while (objects != NULL)
	{
		last_object->object = objects->object;
		last_object->type = objects->type;
		if (objects->next)
		{
			last_object->next = malloc(sizeof(t_object_list));
			last_object = last_object->next;
		}
		objects = objects->next;
	}
	last_object->next = NULL;
	return (new_list);
}

t_light_list	*copy_lights(t_light_list *head)
{
	t_light_list *lights;
	t_light_list *new_lights;
	t_light_list *last_light;

	lights = head;
	new_lights = NULL;
	last_light = NULL;
	if (!(new_lights = malloc(sizeof(t_light_list))))
		return (NULL);
	last_light = new_lights;
	while (lights != NULL)
	{
		last_light->light = lights->light;
		if (lights->next)
		{
			last_light->next = malloc(sizeof(t_light_list));
			last_light = last_light->next;
		}
		lights = lights->next;
	}
	last_light->next = NULL;
	return (new_lights);
}
