/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   composed_constrcutors.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/01 20:17:16 by abenaiss          #+#    #+#             */
/*   Updated: 2020/02/01 20:22:25 by abenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int				ft_add_pill(t_xml_tag *tag, t_rtv *env)
{
	t_object	body;
	t_object	first_edge;
	t_object	second_edge;
	int			status;

	status = 1;
	body.cylinder.center = ft_parse_vector(ft_xml_get_value(tag, "center",
				"(0,0,0)"), &status);
	body.cylinder.translation = ft_parse_vector(ft_xml_get_value(tag,
				"translation", "(0,0,0)"), &status);
	body.cylinder.center = ft_add_vector(body.cylinder.center,
			body.cylinder.translation);
	body.cylinder.radius = ft_parse_float(ft_xml_get_value(tag, "radius",
				"5"), &status);
	body.cylinder.max_lenght = ft_clip_min(-1,
		ft_parse_float(ft_xml_get_value(tag, "lenght", "-1"), &status));
	body.cylinder.axis = ft_parse_vector(ft_xml_get_value(tag, "axis",
				"(0, -1, 0)"), &status);
	first_edge.sphere.center = ft_add_vector(ft_scale_vector(body.cylinder.axis,
		body.cylinder.max_lenght), body.cylinder.center);
	second_edge.sphere.center = ft_sub_vector(ft_scale_vector(body.cylinder.axis,
		body.cylinder.max_lenght), body.cylinder.center);
	first_edge.sphere.radius = body.cylinder.radius;
	second_edge.sphere.radius = body.cylinder.radius;
	first_edge.sphere.color = body.cylinder.color;
	second_edge.sphere.color = body.cylinder.color;
	body.cylinder.function = &ft_cylinder_intersection;
	first_edge.sphere.function = &ft_sphere_intersection;
	second_edge.sphere.function = &ft_sphere_intersection;
	status &= ft_object_push(env, body, TYPE_PLANE);
	status &= ft_object_push(env, first_edge, TYPE_PLANE);
	status &= ft_object_push(env, second_edge, TYPE_PLANE);
	return (status);
}