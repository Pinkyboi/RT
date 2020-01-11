/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reflection.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiri <abiri@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/10 21:35:10 by abiri             #+#    #+#             */
/*   Updated: 2020/01/11 00:01:54 by abiri            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_vector	ft_reflected_ray(t_vector normal, t_vector ray)
{
	t_vector	reflected;
	double		scalar;

	ray = ft_normalise_vector(ft_scale_vector(ray, -1));
	scalar = 2.0 * ft_dot_vector(normal, ray);
	normal = ft_scale_vector(normal, scalar);
	reflected = ft_normalise_vector(ft_sub_vector(normal, ray));
	return (reflected);
}

int		ft_intersect_reflected(t_rtv *rtv)
{
	t_object_list	*node;
	double			temp_min;
	double			min;

	node = rtv->objects;
	min = MAX_D;
	while (node)
	{
		ft_choose_intersection(node, rtv, &min);
		node = node->next;
	}
	if (min != MAX_D)
		return (1);
	return (0);
}

t_color	ft_reflect_ray(t_rtv rtv, t_color *color, int depth)
{
	double	intersection_dist;
	t_color	my_color;
	t_color	other_colors;
	double	reflection;

	(void)color;
	if (depth >= REFLECTION_DEPTH)
		return (*color);
	rtv.cam.position = rtv.cam.hit.position;
	rtv.cam.ray_direction = ft_reflected_ray(rtv.cam.hit.normal,
		rtv.cam.ray_direction);
	reflection = rtv.cam.hit.reflection;
	my_color = ft_scale_colors(ft_mix_colors(&rtv, rtv.cam.hit.normal,
		rtv.cam.hit.color), reflection);
	if (ft_intersect_reflected(&rtv))
		other_colors = ft_scale_colors(ft_reflect_ray(rtv, color, depth + 1),
			1 - reflection);
	else
		other_colors = (t_color){0, 0, 0};
	return (ft_add_colors(my_color, other_colors));
}
