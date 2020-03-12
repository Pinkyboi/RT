/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complex_constructors.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azarzor <azarzor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 08:17:36 by abenaiss          #+#    #+#             */
/*   Updated: 2020/03/04 23:01:16 by azarzor          ###   ########.fr       */
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
	object.holo_cube.radius = ft_clip_min(0, ft_parse_float(
		ft_xml_get_value(tag, "radius", "5"), &status));
	object.holo_cube.color = ft_parse_color(ft_xml_get_value(tag, "color",
				"(255,255,255)"), &status);
	object.holo_cube.translation = ft_parse_vector(ft_xml_get_value(tag,
				"translation", "(0,0,0)"), &status);
	object.holo_cube.center = ft_add_vector(object.holo_cube.center,
			object.holo_cube.translation);
	ft_add_material(tag, &object, &status, env);
	object.cylinder.function = &ft_holo_cube_intersection;
	status &= ft_object_push(env, object, TYPE_HOLO_CUBE);
	return (status);
}

int				ft_add_torus(t_xml_tag *tag, t_rtv *env)
{
	t_object	object;
	int			status;

	status = 1;
	object.torus.center = ft_parse_vector(ft_xml_get_value(tag, "center",
				"(0,0,0)"), &status);
	object.torus.b_radius = ft_clip_min(0, ft_parse_float(
		ft_xml_get_value(tag, "big_radius", "5"), &status));
	object.torus.s_radius = ft_clip_min(0, ft_parse_float(
		ft_xml_get_value(tag, "small_radius", "2"), &status));
	object.torus.color = ft_parse_color(ft_xml_get_value(tag, "color",
				"(255,255,255)"), &status);
	object.torus.translation = ft_parse_vector(ft_xml_get_value(tag,
				"translation", "(0,0,0)"), &status);
	object.torus.center = ft_add_vector(object.torus.center,
			object.torus.translation);
	ft_add_material(tag, &object, &status, env);
	object.torus.function = &ft_torus_intersection;
	status &= ft_object_push(env, object, TYPE_TORUS);
	return (status);
}

int				ft_add_cube(t_xml_tag *tag, t_rtv *env)
{
	t_object	object;
	int			status;
	double		side;

	status = 1;
	side = fabs(ft_parse_float(ft_xml_get_value(tag, "side",
				"10"), &status));
	object.cube.center = ft_parse_vector(ft_xml_get_value(tag, "center",
			"(0, 0, 0)"), &status);
	object.cube.bounds[0] = ft_sub_vector(object.cube.center,
		(t_vector){side / 2, side / 2, side / 2});
	object.cube.bounds[1] = ft_add_vector(object.cube.center,
		(t_vector){side / 2, side / 2, side / 2});
	object.cube.color = ft_parse_color(ft_xml_get_value(tag, "color",
				"(255,255,255)"), &status);
	object.cube.translation = ft_parse_vector(ft_xml_get_value(tag,
				"translation", "(0,0,0)"), &status);
	object.cube.bounds[0] = ft_add_vector(object.cube.bounds[0],
			object.cube.translation);
	object.cube.bounds[1] = ft_add_vector(object.cube.bounds[1],
			object.cube.translation);
	ft_add_material(tag, &object, &status, env);
	object.cube.function = &ft_cube_intersection;
	status &= ft_object_push(env, object, TYPE_CUBE);
	return (status);
}
