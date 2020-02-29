/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filter.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/18 02:09:05 by abiri             #+#    #+#             */
/*   Updated: 2020/02/22 06:06:19 by abenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static t_color	ft_gray_filter(t_color color)
{
	double	light_scale;
	double	gray_scale;

	light_scale = (.2126 * color.r) +
		(.7152 * color.g) + (.0722 * color.b);
	if (light_scale <= 0.0031308)
		gray_scale = 12.92 * light_scale;
	else
		gray_scale = 1.055 * pow(light_scale, 1 / 2.4) - 0.055;
	return (ft_assign_color(gray_scale, gray_scale, gray_scale));
}

static t_color	ft_sepia_filter(t_color color)
{
	double	r;
	double	g;
	double	b;

	r = (color.r * .393) +
		(color.g * .769) + (color.b * .189);
	g = (color.r * .349) +
		(color.g * .686) + (color.b * .168);
	b = (color.r * .272) +
		(color.g * .534) + (color.b * .131);
	return (ft_assign_color(r, g, b));
}

static t_color	ft_outline_effect(t_rtv rtv, t_object object, t_color color)
{
	t_object_list	*object_node;
	t_object_list	*best_node;
	int				sample;

	sample = 0;
	while (++sample < 4)
	{
		object_node = rtv.objects;
		rtv.min = MAX_D;
		best_node = NULL;
		ft_create_ray(&rtv, sample);
		while (object_node)
		{
			if (ft_choose_intersection(object_node, &rtv, &rtv.min))
				best_node = object_node;
			object_node = object_node->next;
		}
		if (best_node)
			if (ft_diff_color(object.point.color,
				best_node->object.point.color))
				color = (t_color){0, 0, 0};
	}
	return (color);
}

t_color			ft_select_filter(t_rtv rtv, t_object object, t_color color)
{
	if (rtv.scene.filter == 1)
		return (ft_gray_filter(color));
	if (rtv.scene.filter == 2)
		return (ft_sepia_filter(color));
	if (rtv.scene.filter == 3)
		return (ft_assign_color(1 - color.r, 1 - color.g, 1 - color.b));
	if (rtv.scene.effect == 4 || rtv.scene.effect == 5)
		return (ft_outline_effect(rtv, object, color));
	if (rtv.scene.filter == 4)
		return ((t_color){color.r, 0, color.b});
	if (rtv.scene.filter == 5)
		return ((t_color){color.r, color.r, color.b});
	if (rtv.scene.filter == 7)
	{
		color = ft_gray_filter(color);
		return ((t_color){0, color.g, color.b});
	}
	if (rtv.scene.filter == 8)
	{
		color = ft_gray_filter(color);
		return ((t_color){color.r, 0, 0});
	}
	return (color);
}
