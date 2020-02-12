/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reflection.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azarzor <azarzor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/10 21:35:10 by abiri             #+#    #+#             */
/*   Updated: 2020/01/21 15:22:54 by azarzor          ###   ########.fr       */
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

int			ft_intersect_reflected(t_rtv *rtv)
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

t_color		ft_reflect_ray(t_rtv rtv, int depth)
{
	t_color	other_colors;
	double	reflection;

	if (!rtv.scene.reflection_depth || depth > rtv.scene.reflection_depth)
		return ((t_color){0, 0, 0});
	rtv.cam.ray_origin = rtv.cam.hit.position;
	rtv.cam.ray_direction = ft_reflected_ray(rtv.cam.hit.normal,
		rtv.cam.ray_direction);
	reflection = rtv.cam.hit.reflection;
	if (ft_intersect_reflected(&rtv))
		return (ft_get_node_color(rtv, depth + 1));
	return ((t_color){0, 0, 0});
}
