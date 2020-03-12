/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   procedural_textures.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiri <abiri@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/16 22:47:55 by abenaiss          #+#    #+#             */
/*   Updated: 2020/02/28 23:33:17 by abenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_color			ft_checker_texture(double x, double y)
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

t_color			ft_pastel(double x, double y)
{
	double	color_noise;
	t_color	color;

	color_noise = ft_turbulence(x, y, 0, 60) * 20;
	color_noise = color_noise - (int)color_noise;
	if (color_noise > 0 && color_noise < 0.2)
		color = (t_color){232.0 / 255.0, 187.0 / 255.0, 201.0 / 255.0};
	else if (color_noise < 0.4)
		color = (t_color){154.0 / 255.0, 62.0 / 255.0, 130.0 / 255.0};
	else if (color_noise < 0.6)
		color = (t_color){140.0 / 255.0, 209.0 / 255.0, 224.0 / 255.0};
	else if (color_noise < 0.8)
		color = (t_color){34.0 / 255.0, 74.0 / 255.0, 142.0 / 255.0};
	else
		color = (t_color){213.0 / 255.0, 119.0 / 255.0, 61.0 / 255.0};
	return (color);
}

t_color			ft_marble(double x, double y)
{
	double	color;
	double	noise_value;

	noise_value = 0;
	noise_value = ft_turbulence(x * 20, y * 20, 0, 6) * 5.0;
	color = (sin((y + noise_value * 100) * 2 * M_PI / 200.f) + 1) / 2.0;
	return ((t_color){color, color, color});
}

t_color			ft_wood(double x, double y)
{
	t_color	color;
	double	x_factor;
	double	y_factor;
	double	noise_value;
	double	wave_value;

	x_factor = (x - 512 / 2) / (double)(512);
	y_factor = (y - 512 / 2) / (double)(512);
	noise_value = sqrt(x_factor * x_factor + y_factor * y_factor)
		+ 2.0 * ft_turbulence(x, y, 0, 20) / 256.0;
	wave_value = 128.0 * fabs(sin(2 * 30.0 * noise_value * 3.14159));
	color.r = (int)(80 + wave_value) / 255.0;
	color.g = (int)(30 + wave_value) / 255.0;
	color.b = 30 / 255.0;
	return (color);
}
