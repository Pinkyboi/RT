/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   basics_constructors.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiri <abiri@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/18 02:09:05 by abiri             #+#    #+#             */
/*   Updated: 2020/02/26 23:53:54 by abiri            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int				ft_add_sphere(t_xml_tag *tag, t_rtv *env)
{
	t_object	object;
	int			status;

	status = 1;
	object.sphere.center = ft_parse_vector(ft_xml_get_value(tag, "center",
				"(0,0,0)"), &status);
	object.sphere.radius = ft_parse_float(ft_xml_get_value(tag, "radius",
				"5"), &status);
	object.sphere.color = ft_parse_color(ft_xml_get_value(tag, "color",
				"(255,255,255)"), &status);
	object.sphere.translation = ft_parse_vector(ft_xml_get_value(tag,
				"translation", "(0,0,0)"), &status);
	object.sphere.center = ft_add_vector(object.sphere.center,
			object.sphere.translation);
	ft_add_material(tag, &object, &status, env);
	ft_sphere_cut(env, tag, &object, &status);
	object.sphere.function = &ft_sphere_intersection;
	status &= ft_object_push(env, object, TYPE_SPHERE);
	return (status);
}

int				ft_add_cylinder(t_xml_tag *tag, t_rtv *env)
{
	t_object	object;
	int			status;

	status = 1;
	object.cylinder.center = ft_parse_vector(ft_xml_get_value(tag, "center",
				"(0,0,0)"), &status);
	object.cylinder.radius = ft_parse_float(ft_xml_get_value(tag, "radius",
				"5"), &status);
	object.cylinder.axis = ft_parse_vector(ft_xml_get_value(tag, "axis",
				"(0, -1, 0)"), &status);
	object.cylinder.color = ft_parse_color(ft_xml_get_value(tag, "color",
				"(255,255,255)"), &status);
	object.cylinder.rotation = ft_parse_vector(ft_xml_get_value(tag, "rotation",
			"(0,0,0)"), &status);
	object.cylinder.translation = ft_parse_vector(ft_xml_get_value(tag,
				"translation", "(0,0,0)"), &status);
	object.cylinder.center = ft_add_vector(object.cylinder.center,
			object.cylinder.translation);
	object.cylinder.axis = ft_normalise_vector(ft_rotate_vector(
				object.cylinder.axis, object.cylinder.rotation));
	ft_add_material(tag, &object, &status, env);
	ft_cylinder_cut(env, tag, &object, &status);
	object.cylinder.function = &ft_cylinder_intersection;
	status &= ft_object_push(env, object, TYPE_CYLINDER);
	return (status);
}

void			ft_get_plane_axis(t_xml_tag *tag,
	t_plane *plane, int *status, t_coor lengths)
{
	plane->rotation = ft_parse_vector(
		ft_xml_get_value(tag, "rotation", "(0,0,0)"), status);
	plane->sides.u = ft_rotate_vector(ft_normalise_vector(ft_parse_vector(
		ft_xml_get_value(tag, "U", "(0, 0, 1)"), status)), plane->rotation);
	plane->sides.v = ft_rotate_vector(ft_normalise_vector(ft_parse_vector(
		ft_xml_get_value(tag, "V", "(1, 0, 0)"), status)), plane->rotation);
	plane->length.u = ft_clip_min(-1, lengths.x);
	plane->length.v = ft_clip_min(-1, lengths.y);
	plane->normal = ft_normalise_vector(
		ft_cross_product(plane->sides.u, plane->sides.v));
}

int				ft_add_plane(t_xml_tag *tag, t_rtv *env)
{
	t_object	object;
	int			status;
	t_coor		lengths;

	status = 1;
	lengths = ft_parse_coor(ft_xml_get_value(tag,
		"length", "(-1, -1)"), &status);
	object.plane.center = ft_parse_vector(ft_xml_get_value(tag, "center",
				"(0,0,0)"), &status);
	object.plane.color = ft_parse_color(ft_xml_get_value(tag, "color",
				"(255,255,255)"), &status);
	object.plane.translation = ft_parse_vector(ft_xml_get_value(tag,
				"translation", "(0,0,0)"), &status);
	object.plane.center = ft_add_vector(object.plane.center,
			object.plane.translation);
	object.plane.radius = ft_clip_min(-1, ft_parse_float(
				ft_xml_get_value(tag, "radius", "-1"), &status));
	ft_get_plane_axis(tag, &object.plane, &status, lengths);
	ft_define_limits(tag, &(object.plane.limits), &status);
	ft_add_material(tag, &object, &status, env);
	object.plane.function = &ft_plane_intersection;
	status &= ft_object_push(env, object, TYPE_PLANE);
	return (status);
}
