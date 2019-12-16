/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/26 07:22:58 by abenaiss          #+#    #+#             */
/*   Updated: 2019/12/15 23:44:08 by abenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

double			ft_choose_intersection(t_object_list *object_node,
					t_rtv *rtv, double *min)
{
	double			temp_min;

	temp_min = object_node->object.point.function(&(rtv->cam),
		&object_node->object.point, min);
	if (temp_min)
		*min = temp_min;
	return (temp_min);
}

double			ft_check_intersection(t_rtv rtv,
			t_vector light_vector, t_vector intersection_point)
{
	t_object_list	*object_node;
	double			min;
	double			temp_min;

	rtv.cam.position = intersection_point;
	rtv.cam.ray_direction = light_vector;
	min = MAX_D;
	object_node = rtv.objects;
	while (object_node)
	{
		temp_min = ft_choose_intersection(object_node, &rtv, &min);
		if (temp_min)
			min = temp_min;
		object_node = object_node->next;
	}
	if (min)
		return (min);
	return (0);
}

void			ft_color_best_node(t_rtv *rtv, t_color rgb)
{
	t_object_list	*object_node;
	t_object_list	*best_node;
	int				sample;

	sample = -1;
	while (++sample < (AA + 1))
	{
		object_node = rtv->objects;
		rtv->min = MAX_D;
		best_node = NULL;
		ft_create_ray(rtv, sample);
		while (object_node)
		{
			if (ft_choose_intersection(object_node, rtv, &rtv->min))
				best_node = object_node;
			object_node = object_node->next;
		}
		if (best_node)
			rgb = ft_add_colors(rgb, ft_mix_colors(rtv,
				best_node->object.point.normal, best_node->object.point.color));
	}
	rgb = ft_scale_colors(rgb, (double)1 / (AA + 1));
	if (best_node)
		ft_put_pixel(rtv, ft_rgb_to_int(rgb));
}

void			ft_ray_shooter(t_rtv *rtv)
{
	t_color	rgb;

	rtv->row = -1;
	while (++rtv->row < WIN_WIDTH)
	{
		rtv->column = -1;
		while (++rtv->column < WIN_HEIGHT)
		{
			rgb = (t_color){0, 0, 0};
			ft_color_best_node(rtv, rgb);
		}
	}
}
