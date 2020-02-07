/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   composed_constructors.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/01 20:17:16 by abenaiss          #+#    #+#             */
/*   Updated: 2020/02/07 05:23:54 by abenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int			ft_add_pill(t_xml_tag *tag, t_rtv *env)
{
	t_object	body;
	t_object	first_edge;
	int			status;

	status = 1;
	body.cylinder.center = ft_parse_vector(ft_xml_get_value(tag, "center",
				"(0,0,0)"), &status);
	body.cylinder.radius = ft_parse_float(ft_xml_get_value(tag, "radius",
				"5"), &status);
	body.cylinder.axis = ft_parse_vector(ft_xml_get_value(tag, "axis",
				"(0, -1, 0)"), &status);
	body.cylinder.color = ft_parse_color(ft_xml_get_value(tag, "color",
				"(255,255,255)"), &status);
	body.cylinder.rotation = ft_parse_vector(ft_xml_get_value(tag, "rotation",
			"(0,0,0)"), &status);
	body.cylinder.axis = ft_normalise_vector(ft_rotate_vector(
				body.cylinder.axis, body.cylinder.rotation));
	body.cylinder.max_lenght = ft_clip_min(1,
		ft_parse_float(ft_xml_get_value(tag, "lenght", "5"), &status));
	ft_add_material(tag, &body, &status);
	ft_define_limits(tag, &(body.cylinder.limits), &status);
	body.cylinder.function = &ft_cylinder_intersection;
	status &= ft_object_push(env, body, TYPE_CYLINDER);
	first_edge.sphere.center = ft_add_vector(ft_scale_vector(body.cylinder.axis,
		body.cylinder.max_lenght / 2), body.cylinder.center);
	first_edge.sphere.radius = body.cylinder.radius;
	first_edge.sphere.color = body.cylinder.color;
	first_edge.sphere.limits = body.cylinder.limits;
	ft_add_material(tag, &first_edge, &status);
	first_edge.sphere.function = &ft_sphere_intersection;
	status &= ft_object_push(env, first_edge, TYPE_SPHERE);
	first_edge.sphere.center = ft_add_vector(ft_scale_vector(body.cylinder.axis,
	-body.cylinder.max_lenght / 2), body.cylinder.center);
	status &= ft_object_push(env, first_edge, TYPE_SPHERE);
	return (status);
}

t_plane		ft_get_side(t_plane *plane, t_vector center,
	t_vector lenghts, t_sides sides)
{
	t_plane second_plane;

	plane->normal = ft_normalise_vector(ft_cross_product(sides.u, sides.v));
	plane->lenght.u = lenghts.x;
	plane->lenght.v = lenghts.y;
	plane->sides.u = sides.u;
	plane->sides.v = sides.v;
	second_plane = *plane;
	plane->center = ft_add_vector(ft_scale_vector(plane->normal,
		lenghts.z), center);
	second_plane.center = ft_add_vector(ft_scale_vector(plane->normal,
		-lenghts.z), center);
	return (second_plane);
}

void		ft_get_edges(t_rtv *env, int *status, t_object *sides, t_vector *cube_utils)
{
	int i;

	i = -1;
	while (++i < 6)
		sides[i] = sides[0];
	sides[1].plane = ft_get_side(&sides[0].plane,
		CENTER, LENGHTS, sides[0].plane.sides);
	sides[3].plane = ft_get_side(&sides[2].plane, CENTER,
		(t_vector){LENGHTS.x, LENGHTS.z, LENGHTS.y},
		(t_sides){sides[0].plane.normal, sides[0].plane.sides.v});
	sides[5].plane = ft_get_side(&sides[4].plane, CENTER,
		(t_vector){LENGHTS.y, LENGHTS.z, LENGHTS.x},
		(t_sides){sides[0].plane.normal, sides[2].plane.normal});
	while (++i <= 12)
		*status &= ft_object_push(env, sides[i - 7], TYPE_PLANE);
}

int			ft_add_cube(t_xml_tag *tag, t_rtv *env)
{
	t_object	sides[6];
	t_vector	cube_utils[2];
	int			status;

	status = 1;
	CENTER = ft_parse_vector(ft_xml_get_value(tag,
		"center", "(10,8,4)"), &status);
	LENGHTS = ft_parse_vector(ft_xml_get_value(tag,
		"lenght", "(10,8,4)"), &status);
	ft_get_plane_axis(tag, &sides[0].plane, &status,
		(t_coor){LENGHTS.x, LENGHTS.y});
	sides[0].plane.color = ft_parse_color(
		ft_xml_get_value(tag, "color", "(255,255,255)"), &status);
	sides[0].plane.radius = -1;
	ft_define_limits(tag, &(sides[0].plane.limits), &status);
	ft_add_material(tag, &sides[0], &status);
	sides[0].plane.function = &ft_plane_intersection;
	ft_get_edges(env, &status, sides, cube_utils);
	return (status);
}
