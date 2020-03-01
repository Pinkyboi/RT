/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loading_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azarzor <azarzor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/05 21:47:13 by abiri             #+#    #+#             */
/*   Updated: 2020/03/01 05:30:28 by azarzor          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include <stdio.h>

const t_xml_element_parse	g_xml_element_parser[] = {
	{.type_name = "sphere", .function = &ft_add_sphere},
	{.type_name = "demisphere", .function = &ft_add_demi_sphere},
	{.type_name = "cylinder", .function = &ft_add_cylinder},
	{.type_name = "plane", .function = &ft_add_plane},
	{.type_name = "disk", .function = &ft_add_disk},
	{.type_name = "cone", .function = &ft_add_cone},
	{.type_name = "ellipsoid", .function = &ft_add_ellipsoid},
	{.type_name = "hyperboloid", .function = &ft_add_hyperboloid},
	{.type_name = "triangle", .function = &ft_add_triangle},
	{.type_name = "paraboloid", .function = &ft_add_paraboloid},
	{.type_name = "pill", .function = &ft_add_pill},
	{.type_name = "box", .function = &ft_add_box},
	{.type_name = "parallelepiped", .function = &ft_add_parallelepiped},
	{.type_name = "holo_cube", .function = &ft_add_holo_cube},
	{.type_name = "torus", .function = &ft_add_torus},
	{.type_name = "cube", .function = &ft_add_cube},
	{.type_name = "camera", .function = &ft_load_camera},
	{.type_name = "scene", .function = &ft_load_scene},
	{.type_name = "light", .function = &ft_add_light},
	{.type_name = "fractal", .function = &ft_add_fractal}
};

t_xml_element				*ft_get_tag_parsing_function(char *name)
{
	size_t	index;

	index = 0;
	while (index < TYPE_PARSE_COUNT)
	{
		if (ft_strequ(name, g_xml_element_parser[index].type_name))
			return (g_xml_element_parser[index].function);
		index++;
	}
	return (NULL);
}

int							ft_load_shapes(t_xml_data *data, t_rtv *env)
{
	t_xml_tag		*tag;
	t_xml_element	*function;
	int				result;

	tag = data->tags;
	while (tag)
	{
		if ((function = ft_get_tag_parsing_function(tag->name)))
			result = function(tag, env);
		if (!result)
		{
			ft_putstr("There is an error in the element :\033[0;31m ");
			ft_putstr(tag->name);
			ft_putstr("\033[0m\n");
			result = 1;
		}
		tag = tag->next;
	}
	return (1);
}

int							ft_light_push(t_rtv *env, t_light light)
{
	t_light_list	*list_node;

	if (!(list_node = ft_memalloc(sizeof(t_light_list))))
		return (0);
	list_node->light = light;
	if (!env->lights || !env->last_light)
	{
		env->lights = list_node;
		env->last_light = list_node;
	}
	else
	{
		env->last_light->next = list_node;
		env->last_light = list_node;
	}
	return (1);
}

int							ft_object_push(t_rtv *env, t_object object,
		int type)
{
	t_object_list	*list_node;

	if (!(list_node = ft_memalloc(sizeof(t_object_list))))
		return (0);
	list_node->object = object;
	list_node->type = type;
	if (!env->objects || !env->last_object)
	{
		env->objects = list_node;
		env->last_object = list_node;
	}
	else
	{
		env->last_object->next = list_node;
		env->last_object = list_node;
	}
	return (1);
}
