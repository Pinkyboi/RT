/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   procedural_textures.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiri <abiri@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/16 22:47:55 by abenaiss          #+#    #+#             */
/*   Updated: 2020/02/27 00:09:36 by abiri            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_color			ft_cheeker_texture(double x, double y)
{
	double scale;

	scale = 10;
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
	double	odd;
	double	edge;
	double	scale;

	x = fabs(x);
	y = fabs(y);
	scale = 1;
	tx = (int)(scale * x);
	ty = (int)(scale * y);
	odd = ((tx & 0x01) == ((ty & 0x01)));
	edge = (((scale * x - tx < 0.1) && odd) ||
		((scale * y - ty < 0.1)));
	return ((edge ? (t_color){1, 1, 1} : (t_color){1, 0, 0}));
}
