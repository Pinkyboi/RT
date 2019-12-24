/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cut_helper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/24 05:58:14 by abenaiss          #+#    #+#             */
/*   Updated: 2019/12/24 05:58:17 by abenaiss         ###   ########.fr       */
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
	"limitX", "(-1000000000000000000000,1000000000000000000000)"), status));
	limit->y_limit = ft_swap_limits(ft_parse_coor(
		ft_xml_get_value(tag,
	"limitY", "(-1000000000000000000000,1000000000000000000000)"), status));
	limit->z_limit = ft_swap_limits(ft_parse_coor(
		ft_xml_get_value(tag,
	"limitZ", "(-1000000000000000000000,1000000000000000000000)"), status));
}
