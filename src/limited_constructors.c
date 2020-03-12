/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   limited_constructors.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azarzor <azarzor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/07 23:21:45 by abenaiss          #+#    #+#             */
/*   Updated: 2020/03/04 23:11:18 by azarzor          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int				ft_add_demi_sphere(t_xml_tag *tag, t_rtv *env)
{
	t_object	object;
	int			status;

	status = 1;
	object.sphere.center = ft_parse_vector(ft_xml_get_value(tag, "center",
				"(0,0,0)"), &status);
	object.sphere.radius = ft_clip_min(1, ft_parse_float(
		ft_xml_get_value(tag, "radius", "5"), &status));
	object.sphere.color = ft_parse_color(ft_xml_get_value(tag, "color",
				"(255,255,255)"), &status);
	object.sphere.translation = ft_parse_vector(ft_xml_get_value(tag,
				"translation", "(0,0,0)"), &status);
	object.sphere.rotation = ft_parse_vector(ft_xml_get_value(tag, "rotation",
			"(0,0,0)"), &status);
	object.sphere.center = ft_add_vector(object.sphere.center,
			object.sphere.translation);
	ft_add_material(tag, &object, &status, env);
	ft_demi_sphere_cut(env, tag, &object, &status);
	object.sphere.function = &ft_sphere_intersection;
	status &= ft_object_push(env, object, TYPE_SPHERE);
	return (status);
}

int				ft_add_triangle(t_xml_tag *tag, t_rtv *env)
{
	t_object	object;
	int			status;

	status = 1;
	object.triangle.center = ft_parse_vector(ft_xml_get_value(tag, "center",
				"(0,0,0)"), &status);
	object.triangle.color = ft_parse_color(ft_xml_get_value(tag, "color",
				"(255,255,255)"), &status);
	object.triangle.translation = ft_parse_vector(ft_xml_get_value(tag,
				"translation", "(0,0,0)"), &status);
	object.triangle.center = ft_add_vector(object.triangle.center,
			object.plane.translation);
	ft_add_material(tag, &object, &status, env);
	ft_sides_handle(tag, &object, &status);
	object.triangle.function = &ft_triangle_intersection;
	status &= ft_object_push(env, object, TYPE_TRIANGLE);
	return (status);
}

int				ft_add_disk(t_xml_tag *tag, t_rtv *env)
{
	t_object	object;
	int			status;

	status = 1;
	object.plane.center = ft_parse_vector(ft_xml_get_value(tag, "center",
				"(0,0,0)"), &status);
	object.plane.normal = ft_parse_vector(ft_xml_get_value(tag, "normal",
				"(0,0,0)"), &status);
	object.plane.color = ft_parse_color(ft_xml_get_value(tag, "color",
				"(255,255,255)"), &status);
	object.plane.rotation = ft_parse_vector(ft_xml_get_value(tag, "rotation",
			"(0,0,0)"), &status);
	object.plane.translation = ft_parse_vector(ft_xml_get_value(tag,
				"translation", "(0,0,0)"), &status);
	object.plane.center = ft_add_vector(object.plane.center,
			object.plane.translation);
	object.plane.normal = ft_normalise_vector(
		ft_rotate_vector(object.plane.normal, object.plane.rotation));
	object.plane.radius = ft_clip_min(0, ft_parse_float(
		ft_xml_get_value(tag, "radius", "6"), &status));
	ft_add_material(tag, &object, &status, env);
	ft_define_limits(tag, &(object.plane.limits), &status);
	object.plane.function = &ft_plane_intersection;
	status &= ft_object_push(env, object, TYPE_PLANE);
	return (status);
}
