/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constructors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/18 02:09:05 by abiri             #+#    #+#             */
/*   Updated: 2019/12/16 06:19:28 by abenaiss         ###   ########.fr       */
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
	object.cylinder.limited = ft_parse_float(ft_xml_get_value(tag, "limited",
				"0"), &status);
	object.cylinder.max_lenght = ft_parse_float(ft_xml_get_value(tag, "lenght",
				"0"), &status);
	object.point.function = &ft_cylinder_intersection;
	status &= ft_object_push(env, object, TYPE_CYLINDER);
	return (status);
}

int				ft_add_plane(t_xml_tag *tag, t_rtv *env)
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
	object.plane.normal = ft_rotate_vector(object.plane.normal,
			object.plane.rotation);
	object.plane.normal = ft_normalise_vector(object.plane.normal);
	object.point.function = &ft_plane_intersection;
	status &= ft_object_push(env, object, TYPE_PLANE);
	return (status);
}

int				ft_add_cone(t_xml_tag *tag, t_rtv *env)
{
	t_object	object;
	int			status;

	status = 1;
	object.cone.center = ft_parse_vector(ft_xml_get_value(tag, "center",
				"(0,0,0)"), &status);
	object.cone.axis = ft_parse_vector(ft_xml_get_value(tag, "axis",
				"(0, -1, 0)"), &status);
	object.cone.color = ft_parse_color(ft_xml_get_value(tag, "color",
				"(255,255,255)"), &status);
	object.cone.angle = FT_RAD(ft_parse_float(ft_xml_get_value(tag, "angle",
				"30"), &status));
	object.cone.tilt = tan(object.cone.angle / 2);
	object.cone.rotation = ft_parse_vector(ft_xml_get_value(tag, "rotation",
				"(0,0,0)"), &status);
	object.cone.translation = ft_parse_vector(ft_xml_get_value(tag,
				"translation", "(0,0,0)"), &status);
	object.cone.center = ft_add_vector(object.cone.center,
			object.cone.translation);
	object.cone.axis = ft_normalise_vector(ft_rotate_vector(object.cone.axis,
				object.cone.rotation));
	object.cone.limited = ft_parse_float(ft_xml_get_value(tag, "limited",
				"0"), &status);
	object.cone.max_lenght = ft_parse_float(ft_xml_get_value(tag, "lenght",
				"0"), &status);
	object.point.function = &ft_cone_intersection;
	status &= ft_object_push(env, object, TYPE_CONE);
	return (status);
}
