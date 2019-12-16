/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/19 18:58:42 by abenaiss          #+#    #+#             */
/*   Updated: 2019/12/06 21:25:20 by abenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int			ft_rgb_to_int(t_color color)
{
	int c;
	int g;
	int b;

	g = ft_clamp_max(255, color.g * 255);
	b = ft_clamp_max(255, color.b * 255);
	c = ft_clamp_max(255, color.r * 255);
	c = (c << 8) | g;
	c = (c << 8) | b;
	return (ft_clamp_min(0, c));
}

double		ft_clamp_min(int min, double value)
{
	return ((value >= min) ? value : min);
}

double		ft_clamp_max(int max, double value)
{
	return ((value <= max) ? value : max);
}

double		ft_clamp_min_max(int min, int max, double value)
{
	return (ft_clamp_max(max, ft_clamp_min(min, value)));
}
