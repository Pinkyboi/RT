/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cut_helper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/24 05:58:14 by abenaiss          #+#    #+#             */
/*   Updated: 2020/01/12 19:43:43 by abenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_plane			ft_define_plane(t_vector center, t_vector normal,
					t_color color, double radius)
{
	t_plane plane;

	plane.center = center;
	plane.normal = normal;
	plane.color = color;
	plane.radius = radius;
	plane.reflection = 1;
	return (plane);
}

t_coor			ft_swap_limits(t_coor limits)
{
	double	min_save;

	if (limits.x > limits.y)
	{
		min_save = limits.x;
		limits.x = limits.y;
		limits.y = min_save;
	}
	return (limits);
}

void			ft_define_limits(t_xml_tag *tag,
	t_limit *limit, int *status)
{
	limit->x_limit = ft_swap_limits(ft_parse_coor(
		ft_xml_get_value(tag,
	"limit_x", "(-1000000000000000000000,1000000000000000000000)"), status));
	limit->y_limit = ft_swap_limits(ft_parse_coor(
		ft_xml_get_value(tag,
	"limit_y", "(-1000000000000000000000,1000000000000000000000)"), status));
	limit->z_limit = ft_swap_limits(ft_parse_coor(
		ft_xml_get_value(tag,
	"limit_z", "(-1000000000000000000000,1000000000000000000000)"), status));
}

void			ft_sides_handle(t_xml_tag *tag,
	t_object *object, int *status)
{
	t_vector vertex_edge[2];

	vertex_edge[0] = ft_parse_vector(ft_xml_get_value(tag,
		"side1", "(10,0,0)"), status);
	vertex_edge[1] = ft_parse_vector(ft_xml_get_value(tag,
		"side2", "(0,10,0)"), status);
	ft_define_limits(tag, &(object->triangle.limits), status);
	object->triangle.side[0] = ft_sub_vector(vertex_edge[0],
		object->triangle.center);
	object->triangle.side[1] = ft_sub_vector(vertex_edge[1],
		object->triangle.center);
	object->triangle.normal = ft_normalise_vector(
		ft_cross_product(object->triangle.side[1], object->triangle.side[0]));
}
