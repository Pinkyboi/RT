/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parallelepiped_constructor.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/07 21:44:00 by abenaiss          #+#    #+#             */
/*   Updated: 2020/02/08 04:47:55 by abenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_plane		ft_get_parallelepiped_side(t_plane *plane, t_vector center,
	t_vector lenghts, t_sides sides)
{
	t_plane second_plane;
	t_vector	axis;

	plane->normal = ft_normalise_vector(ft_cross_product(sides.u, sides.v));
	axis = sides.w;
	plane->lenght.u = lenghts.x;
	plane->lenght.v = lenghts.y;
	plane->sides.u = sides.u;
	plane->sides.v = sides.v;
	plane->sides.w = sides.w;
	second_plane = *plane;
	plane->center = ft_add_vector(ft_scale_vector(axis,
		lenghts.z), center);
	second_plane.center = ft_add_vector(ft_scale_vector(axis,
		-lenghts.z), center);
	return (second_plane);
}

void		ft_get_parallelepiped_edges(t_rtv *env, int *status, t_object *sides, t_vector *cube_utils)
{
	int i;

	i = -1;
	while (++i < 6)
		sides[i] = sides[0];
	sides[1].plane = ft_get_parallelepiped_side(&sides[0].plane,
		CENTER, LENGHTS, sides[0].plane.sides);
	sides[3].plane = ft_get_parallelepiped_side(&sides[2].plane, CENTER,
		(t_vector){LENGHTS.z, LENGHTS.y, LENGHTS.x},
		(t_sides){sides[0].plane.sides.u, sides[0].plane.sides.w, sides[0].plane.sides.v});
	sides[5].plane = ft_get_parallelepiped_side(&sides[4].plane, CENTER,
		(t_vector){LENGHTS.z * 0.71,LENGHTS.x * 0.71, LENGHTS.y},
		(t_sides){sides[2].plane.normal, sides[0].plane.normal, sides[0].plane.sides.u});
	while (++i <= 12)
		*status &= ft_object_push(env, sides[i - 7], TYPE_PLANE);
}

int			ft_add_parallelepiped(t_xml_tag *tag, t_rtv *env)
{
	t_object	sides[6];
	t_vector	cube_utils[2];
	int			status;

	status = 1;
	CENTER = ft_parse_vector(ft_xml_get_value(tag, "center",
			"(0,0,0)"), &status);
	LENGHTS = ft_parse_vector(ft_xml_get_value(tag,
		"lenght", "(2,2,2)"), &status);
	sides[0].plane.sides.u = ft_rotate_vector((t_vector){1, 0, 0}, sides[0].plane.rotation);
	sides[0].plane.sides.v = ft_rotate_vector((t_vector){0, 0, 1}, sides[0].plane.rotation);
	sides[0].plane.sides.w = ft_rotate_vector((t_vector){0, 0.7071, 0.7071},sides[0].plane.rotation);
	sides[0].plane.lenght.u = ft_clip_min(-1, LENGHTS.x);
	sides[0].plane.lenght.v = ft_clip_min(-1, LENGHTS.y);
	sides[0].plane.lenght.w = ft_clip_min(-1, LENGHTS.z);
	sides[0].plane.color = ft_parse_color(
		ft_xml_get_value(tag, "color", "(255,255,255)"), &status);
	sides[0].plane.radius = -1;
	ft_define_limits(tag, &(sides[0].plane.limits), &status);
	ft_add_material(tag, &sides[0], &status);
	sides[0].plane.function = &ft_plane_intersection;
	ft_get_parallelepiped_edges(env, &status, sides, cube_utils);
	return (status);
}
