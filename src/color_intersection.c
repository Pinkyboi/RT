/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_intersection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azarzor <azarzor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 00:09:38 by abenaiss          #+#    #+#             */
/*   Updated: 2020/03/04 21:50:44 by azarzor          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

double			ft_choose_intersection(t_object_list *object_node,
					t_rtv *rtv, double *min)
{
	double			temp_min;

	rtv->cam.hit.uv = (t_coor){0, 0};
	temp_min = object_node->object.point.function(&(rtv->cam),
		&object_node->object.point, *min);
	if (temp_min)
	{
		rtv->cam.hit.object = &object_node->object;
		*min = temp_min;
	}
	return (temp_min);
}

t_color			ft_get_node_color(t_rtv rtv, int depth)
{
	t_color	node_color;
	t_color	reflection_color;
	t_color reflection_and_node;
	t_color	refraction_color;

	node_color = ft_scale_colors(
		ft_mix_colors(&rtv, rtv.cam.hit.normal, rtv.cam.hit.color),
		rtv.cam.hit.reflection);
	reflection_color = ft_scale_colors(
		ft_reflect_ray(rtv, depth),
		1 - rtv.cam.hit.reflection);
	reflection_and_node = ft_add_colors(node_color, reflection_color);
	refraction_color = ft_refract_ray(rtv, depth);
	if (rtv.cam.hit.refraction == 0)
		return (reflection_and_node);
	return (ft_add_colors(ft_scale_colors(reflection_and_node,
		1.0 - rtv.cam.hit.transparency), ft_scale_colors(refraction_color,
		rtv.cam.hit.transparency)));
}

void			ft_color_best_node(t_rtv *rtv, t_color rgb)
{
	t_object_list	*object_node;
	t_object_list	*best_node;
	int				sample;

	sample = -1;
	while (++sample < rtv->anti_aliasing + 1)
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
			rgb = ft_add_colors(rgb, ft_get_node_color(*rtv, 1));
	}
	rgb = ft_scale_colors(rgb, (double)1 / (rtv->anti_aliasing + 1));
	(best_node) ? ft_put_pixel(rtv, ft_rgb_to_int(ft_select_filter(
	*rtv, best_node->object, rgb))) : ft_put_pixel(rtv, 0x0);
}

void			ft_color_best_node_dof(t_rtv *rtv, t_color rgb)
{
	t_object_list	*object_node;
	t_object_list	*best_node;
	int				sample;

	sample = -1;
	while (++sample < rtv->scene.dof + 1)
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
			rgb = ft_add_colors(rgb, ft_get_node_color(*rtv, 1));
	}
	rgb = ft_scale_colors(rgb, (double)1 / (rtv->scene.dof + 1));
	(best_node) ? ft_put_pixel(rtv, ft_rgb_to_int(ft_select_filter(
	*rtv, best_node->object, rgb))) : ft_put_pixel(rtv, 0x0);
}

void			ft_specific_ray_shoot(t_rtv *rtv)
{
	t_object_list	*object_node;

	object_node = rtv->objects;
	rtv->min = MAX_D;
	ft_create_ray(rtv, 0);
	while (object_node)
	{
		ft_choose_intersection(object_node, rtv, &rtv->min);
		object_node = object_node->next;
	}
}
