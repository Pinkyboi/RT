/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filter.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/18 02:09:05 by abiri             #+#    #+#             */
/*   Updated: 2019/12/15 05:38:33 by abenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include <stdio.h>

void	ft_assign_color(double r, double g,
	double b, t_color *color)
{
	color->r = r;
	color->g = g;
	color->b = b;
}

void	ft_gray_filter(t_color *color)
{
	double	light_scale;
	double	gray_scale;

	light_scale = (.2126 * color->r) +
		(.7152 * color->g) + (.0722 * color->b);
	if (light_scale <= 0.0031308)
		gray_scale = 12.92 * light_scale;
	else
		gray_scale = 1.055 * pow(light_scale, 1 / 2.4) - 0.055;
	ft_assign_color(gray_scale, gray_scale, gray_scale, color);
}

void	ft_negatif_filter(t_color *color)
{
	ft_assign_color(1 - color->r, 1 - color->g,
		1 - color->b, color);
}

void	ft_sepia_filter(t_color *color)
{
	double	r;
	double	g;
	double	b;

	r = (color->r * .393) +
		(color->g * .769) + (color->b * .189);
	g = (color->r * .349) +
		(color->g * .686) + (color->b * .168);
	b = (color->r * .272) +
		(color->g * .534) + (color->b * .131);
	ft_assign_color(r, g, b, color);
}
