/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   procedural_textures.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/16 22:47:55 by abenaiss          #+#    #+#             */
/*   Updated: 2020/02/07 05:43:04 by abenaiss         ###   ########.fr       */
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

t_color ft_noise_2(t_rtv rtv, t_color color);

t_color			ft_map_texture(t_cam *cam, t_plane plane)
{
	double		x[2];

	x[0] = ft_dot_vector(
			ft_sub_vector(cam->hit.position, plane.center), plane.sides.u);
	x[1] = ft_dot_vector(
			ft_sub_vector(cam->hit.position, plane.center), plane.sides.v);
	return (ft_cheeker_texture(x[1], x[0], 0.1));
}
