/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/19 18:58:42 by abenaiss          #+#    #+#             */
/*   Updated: 2020/02/15 08:27:04 by abenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int			ft_rgb_to_int(t_color color)
{
	int c;
	int g;
	int b;

	g = ft_clip_max(255, color.g * 255);
	b = ft_clip_max(255, color.b * 255);
	c = ft_clip_max(255, color.r * 255);
	c = (c << 8) | g;
	c = (c << 8) | b;
	return (ft_clip_min(0, c));
}

t_color		ft_int_to_rgb(int color)
{
	t_color rgb;

	rgb.r = color / FT_SQR(256);
	rgb.g = (color / 256) % 256;
	rgb.b = color % 256;
	return (rgb);
}

double		ft_clip_min(int min, double value)
{
	return ((value >= min) ? value : min);
}

double		ft_clip_max(int max, double value)
{
	return ((value <= max) ? value : max);
}

double		ft_clip_min_max(int min, int max, double value)
{
	return (ft_clip_max(max, ft_clip_min(min, value)));
}
