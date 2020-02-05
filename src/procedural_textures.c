/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   procedural_textures.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/16 22:47:55 by abenaiss          #+#    #+#             */
/*   Updated: 2020/01/16 22:48:39 by abenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_color			ft_cheeker_texture(double x, double y, double scale)
{
	if ((double)((x * scale)) - floor((x * scale)) < 0.5 ||
		((double)(y * scale)) - floor((double)(y * scale)) < 0.5)
	{
		if ((double)((x * scale)) - floor((x * scale)) < 0.5 &&
			((double)(y * scale)) - floor((y * scale)) < 0.5)
			return ((t_color){0, 0, 0});
		else
			return ((t_color){1, 1, 1});
	}
	else
		return ((t_color){0, 0, 0});
}

t_color			ft_brick_texture(double x, double y)
{
	int		tx;
	int		ty;
	double	oddity;
	double	edge;

	tx = (int)(4 * x);
	ty = (int)(4 * y);
	oddity = ((tx & 0x01) == ((ty & 0x01)) ? 1 : 0);
	edge = ((((4.0 * x) - tx < 0.1) && oddity) ||
		(((4.0 * y) - ty < 0.1) && oddity)) ? 1 : 0;
	return ((edge ? (t_color){1, 1, 1} : (t_color){1, 0, 0}));
}

t_color			ft_map_texture(t_cam *cam, t_plane plane)
{
	t_vector	up;
	t_vector	sides[2];
	double		x[2];

	up = ft_cross_product(ft_rotate_vector(plane.normal,
		(t_vector){90, 90, 90}), plane.normal);
	sides[0] = ft_normalise_vector(
			ft_cross_product(up, plane.normal));
	sides[1] = ft_normalise_vector(
			ft_cross_product(sides[0], plane.normal));
	x[0] = ft_dot_vector(
			ft_sub_vector(cam->hit.position, plane.center), plane.sides.u);
	x[1] = ft_dot_vector(
			ft_sub_vector(cam->hit.position, plane.center), plane.sides.v);
	return (ft_cheeker_texture(x[1], x[0], 0.1));
}
