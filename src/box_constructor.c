/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   box_constructor.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/01 20:17:16 by abenaiss          #+#    #+#             */
/*   Updated: 2020/02/08 04:49:11 by abenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

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

int			ft_add_box(t_xml_tag *tag, t_rtv *env)
{
	t_object	sides[6];
	t_vector	cube_utils[2];
	int			status;

	status = 1;
	CENTER = ft_parse_vector(ft_xml_get_value(tag,
		"center", "(0,0,0)"), &status);
	LENGHTS = ft_parse_vector(ft_xml_get_value(tag,
		"lenght", "(10, 10, 10)"), &status);
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
