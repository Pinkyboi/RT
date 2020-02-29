/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors_operations.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 22:04:25 by azarzor           #+#    #+#             */
/*   Updated: 2020/01/01 02:50:08 by abenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_color	ft_add_colors(t_color first, t_color second)
{
	t_color	result;

	result.r = first.r + second.r;
	result.g = first.g + second.g;
	result.b = first.b + second.b;
	return (result);
}

t_color	ft_scale_colors(t_color first, double scalar)
{
	t_color	result;

	result.r = first.r * scalar;
	result.g = first.g * scalar;
	result.b = first.b * scalar;
	return (result);
}

int		ft_diff_color(t_color c1, t_color c2)
{
	return ((c1.r != c2.r || c1.g != c2.g
		|| c1.b != c2.b) ? 1 : 0);
}

t_color	ft_assign_color(double r, double g, double b)
{
	t_color color;

	color.r = r;
	color.g = g;
	color.b = b;
	return (color);
}

t_color	ft_merge_color(t_color first, t_color second)
{
	t_color result;

	result.r = (first.r < second.r) ? first.r : second.r;
	result.g = (first.g < second.g) ? first.g : second.g;
	result.b = (first.b < second.b) ? first.b : second.b;
	return (result);
}
