/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complex_constructors.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 08:17:36 by abenaiss          #+#    #+#             */
/*   Updated: 2020/02/29 08:17:38 by abenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int				ft_add_holo_cube(t_xml_tag *tag, t_rtv *env)
{
	t_object	object;
	int			status;

	status = 1;
	object.holo_cube.center = ft_parse_vector(ft_xml_get_value(tag, "center",
				"(0,0,0)"), &status);
	object.holo_cube.radius = ft_parse_float(ft_xml_get_value(tag, "radius",
				"5"), &status);
	object.holo_cube.color = ft_parse_color(ft_xml_get_value(tag, "color",
				"(255,255,255)"), &status);
	object.holo_cube.translation = ft_parse_vector(ft_xml_get_value(tag,
				"translation", "(0,0,0)"), &status);
	object.holo_cube.center = ft_add_vector(object.holo_cube.center,
			object.holo_cube.translation);
	ft_add_material(tag, &object, &status, env);
	object.holo_cube.function = &ft_holo_cube_intersection;
	status &= ft_object_push(env, object, TYPE_HOLO_CUBE);
	return (status);
}
