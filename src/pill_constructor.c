/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pill_constructor.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/15 08:30:02 by abenaiss          #+#    #+#             */
/*   Updated: 2020/02/15 08:31:23 by abenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static int	ft_get_pill_edges(t_xml_tag *tag, t_rtv *env, t_object *body)
{
	t_object	edges;
	int			status;

	status = 1;
	edges.sphere.center = ft_add_vector(ft_scale_vector(body->cylinder.axis,
	body->cylinder.max_length / 2), body->cylinder.center);
	edges.sphere.limits = body->cylinder.limits;
	edges.sphere.radius = body->cylinder.radius;
	edges.sphere.color = body->cylinder.color;
	edges.sphere.limits = body->cylinder.limits;
	ft_add_material(tag, &edges, &status, env);
	edges.sphere.function = &ft_sphere_intersection;
	status &= ft_object_push(env, edges, TYPE_SPHERE);
	edges.sphere.center = ft_add_vector(ft_scale_vector(body->cylinder.axis,
	-body->cylinder.max_length / 2), body->cylinder.center);
	status &= ft_object_push(env, edges, TYPE_SPHERE);
	return (status);
}

int			ft_add_pill(t_xml_tag *tag, t_rtv *env)
{
	t_object	body;
	int			status;

	status = 1;
	body.cylinder.center = ft_parse_vector(ft_xml_get_value(tag, "center",
				"(0,0,0)"), &status);
	body.cylinder.radius = ft_clip_min(1, ft_parse_float(
		ft_xml_get_value(tag, "radius", "5"), &status));
	body.cylinder.axis = ft_parse_vector(ft_xml_get_value(tag, "axis",
				"(0, -1, 0)"), &status);
	body.cylinder.color = ft_parse_color(ft_xml_get_value(tag, "color",
				"(255,255,255)"), &status);
	body.cylinder.rotation = ft_parse_vector(ft_xml_get_value(tag, "rotation",
			"(0,0,0)"), &status);
	body.cylinder.axis = ft_normalise_vector(ft_rotate_vector(
				body.cylinder.axis, body.cylinder.rotation));
	body.cylinder.max_length = ft_clip_min(1,
	ft_parse_float(ft_xml_get_value(tag, "length", "5"), &status));
	ft_add_material(tag, &body, &status, env);
	ft_define_limits(tag, &(body.cylinder.limits), &status);
	body.cylinder.function = &ft_cylinder_intersection;
	status &= ft_object_push(env, body, TYPE_CYLINDER);
	status = ft_get_pill_edges(tag, env, &body);
	return (status);
}
