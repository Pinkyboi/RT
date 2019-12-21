/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constructors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/18 02:09:05 by abiri             #+#    #+#             */
/*   Updated: 2019/12/21 17:55:04 by abenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_plane			ft_define_plane(t_vector center, t_vector normal, t_color color, double radius)
{
	t_plane plane;

	plane.center = center;
	plane.normal = normal;
	plane.color	 = color;
	plane.radius = radius;

	return(plane);
}

int				ft_add_sphere(t_xml_tag *tag, t_rtv *env)
{
	t_object	object;
	t_object	complement;
	t_vector	cut_center;
	double		new_radius;
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
	object.sphere.limit = ft_parse_vector(ft_xml_get_value(tag,
				"limit", "(0,0,0)"), &status);
	object.sphere.max_lenght = ft_clamp_min(-1, ft_parse_float(ft_xml_get_value(tag, "lenght",
				"-1"), &status));
	object.sphere.cut_orientation = ft_normalise_vector(ft_parse_vector(ft_xml_get_value(tag,
				"orientation", "(0,1,0)"), &status));
	if(object.sphere.max_lenght > 0 && object.sphere.max_lenght < 2 * object.sphere.radius)
	{
		cut_center = ft_add_vector(ft_scale_vector(object.sphere.cut_orientation,
			- object.sphere.radius + object.sphere.max_lenght),object.sphere.center);
		new_radius = sqrt(FT_SQR(object.sphere.radius) -
			FT_SQR(ft_vector_size(ft_sub_vector(cut_center, object.sphere.center))));
		complement.plane = ft_define_plane(cut_center, 
			object.sphere.cut_orientation, object.sphere.color,new_radius);
		complement.point.function = &ft_plane_intersection;
		status &= ft_object_push(env, complement, TYPE_PLANE);
		
	}
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
	object.cylinder.max_lenght = ft_clamp_min(-1, ft_parse_float(ft_xml_get_value(tag, "lenght",
				"-1"), &status));
	object.cylinder.limit = ft_parse_vector(ft_xml_get_value(tag,
				"limit", "(0,0,0)"), &status);
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
	object.plane.radius = ft_clamp_min(-1, ft_parse_float(ft_xml_get_value(tag, "radius",
				"-1"), &status));
	object.plane.limit = ft_parse_vector(ft_xml_get_value(tag,
				"limit", "(0,0,0)"), &status);
	object.point.function = &ft_plane_intersection;
	status &= ft_object_push(env, object, TYPE_PLANE);
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
	object.plane.normal = ft_rotate_vector(object.plane.normal,
			object.plane.rotation);
	object.plane.normal = ft_normalise_vector(object.plane.normal);
	object.plane.radius = ft_clamp_min(0, ft_parse_float(ft_xml_get_value(tag, "radius",
				"6"), &status));
	object.plane.limit = ft_parse_vector(ft_xml_get_value(tag,
				"limit", "(0,0,0)"), &status);
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
	object.cone.max_lenght = ft_clamp_min(-1, ft_parse_float(ft_xml_get_value(tag, "lenght",
				"-1"), &status));
	object.cone.limit = ft_parse_vector(ft_xml_get_value(tag,
				"limit", "(0,0,0)"), &status);
	object.point.function = &ft_cone_intersection;
	status &= ft_object_push(env, object, TYPE_CONE);
	return (status);
}
