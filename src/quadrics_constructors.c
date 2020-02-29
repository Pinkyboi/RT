/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quadrics_constructors.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiri <abiri@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/18 02:09:05 by abiri             #+#    #+#             */
/*   Updated: 2020/02/14 12:52:21 by abiri            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int				ft_add_ellipsoid(t_xml_tag *tag, t_rtv *env)
{
	t_object	object;
	int			status;

	status = 1;
	object.ellipsoid.center = ft_parse_vector(ft_xml_get_value(tag, "center",
				"(0,0,0)"), &status);
	object.ellipsoid.axis = ft_parse_vector(ft_xml_get_value(tag, "axis",
				"(3,3,3)"), &status);
	object.ellipsoid.axis = (t_vector){fabs(object.ellipsoid.axis.x),
		fabs(object.ellipsoid.axis.y), fabs(object.ellipsoid.axis.z)};
	object.ellipsoid.color = ft_parse_color(ft_xml_get_value(tag, "color",
				"(255,255,255)"), &status);
	object.ellipsoid.translation = ft_parse_vector(ft_xml_get_value(tag,
				"translation", "(0,0,0)"), &status);
	object.ellipsoid.center = ft_add_vector(object.ellipsoid.center,
			object.ellipsoid.translation);
	ft_add_material(tag, &object, &status, env);
	ft_define_limits(tag, &(object.ellipsoid.limits), &status);
	object.ellipsoid.function = &ft_ellipsoid_intersection;
	status &= ft_object_push(env, object, TYPE_ELLIPSOID);
	return (status);
}

int				ft_add_hyperboloid(t_xml_tag *tag, t_rtv *env)
{
	t_object	object;
	int			status;

	status = 1;
	object.hyperboloid.center = ft_parse_vector(ft_xml_get_value(tag, "center",
				"(0,0,0)"), &status);
	object.hyperboloid.coefficient = fabs(ft_parse_float(
				ft_xml_get_value(tag, "coefficient", "10"), &status));
	object.hyperboloid.sheets = ft_clip_min_max(1, 2,
				ft_parse_float(ft_xml_get_value(tag, "sheets", "1"), &status));
	object.hyperboloid.color = ft_parse_color(ft_xml_get_value(tag, "color",
				"(255,255,255)"), &status);
	object.hyperboloid.translation = ft_parse_vector(ft_xml_get_value(tag,
				"translation", "(0,0,0)"), &status);
	object.hyperboloid.center = ft_add_vector(object.hyperboloid.center,
		object.hyperboloid.translation);
	ft_add_material(tag, &object, &status, env);
	ft_define_limits(tag, &(object.hyperboloid.limits), &status);
	object.hyperboloid.function = &ft_hyperboloid_intersection;
	status &= ft_object_push(env, object, TYPE_CYLINDER);
	return (status);
}

int				ft_add_paraboloid(t_xml_tag *tag, t_rtv *env)
{
	t_object	object;
	int			status;

	status = 1;
	object.paraboloid.center = ft_parse_vector(ft_xml_get_value(tag, "center",
				"(0,0,0)"), &status);
	object.paraboloid.coefficient = fabs(ft_parse_float(
				ft_xml_get_value(tag, "coefficient", "10"), &status));
	object.paraboloid.color = ft_parse_color(ft_xml_get_value(tag, "color",
				"(255,255,255)"), &status);
	object.paraboloid.translation = ft_parse_vector(ft_xml_get_value(tag,
				"translation", "(0,0,0)"), &status);
	object.paraboloid.center = ft_add_vector(object.paraboloid.center,
			object.paraboloid.translation);
	ft_add_material(tag, &object, &status, env);
	ft_define_limits(tag, &(object.paraboloid.limits), &status);
	object.paraboloid.function = &ft_paraboloid_intersection;
	status &= ft_object_push(env, object, TYPE_PARABALOID);
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
	object.cone.angle = ft_parse_float(ft_xml_get_value(tag, "angle",
				"30"), &status);
	object.cone.tilt = tan(FT_RAD((object.cone.angle / 2)));
	object.cone.rotation = ft_parse_vector(ft_xml_get_value(tag, "rotation",
				"(0,0,0)"), &status);
	object.cone.center = ft_add_vector(object.cone.center, ft_parse_vector(
	ft_xml_get_value(tag, "translation", "(0,0,0)"), &status));
	object.cone.axis = ft_normalise_vector(ft_rotate_vector(object.cone.axis,
				object.cone.rotation));
	ft_add_material(tag, &object, &status, env);
	ft_cone_cut(env, tag, &object, &status);
	object.cone.function = &ft_cone_intersection;
	status &= ft_object_push(env, object, TYPE_CONE);
	return (status);
}
