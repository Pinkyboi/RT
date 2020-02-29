/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   refraction.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiri <abiri@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/11 20:48:50 by abiri             #+#    #+#             */
/*   Updated: 2020/02/23 03:36:13 by abiri            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static void	ft_swap(double *n1, double *n2)
{
	double temp;

	temp = *n1;
	*n1 = *n2;
	*n2 = temp;
}

static int	ft_intersect_refracted(t_rtv *rtv)
{
	t_object_list	*node;
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

t_vector	ft_get_refracted_ray(t_rtv rtv)
{
	double		refraction_fractors[5];
	t_vector	refraction_vectors[4];

	WORLD_FACT = 1;
	OBJECT_FACT = rtv.cam.hit.refraction;
	DOT_N_R = ft_dot_vector(rtv.cam.hit.normal, rtv.cam.ray_direction);
	NORMAL_REF = rtv.cam.hit.normal;
	if (DOT_N_R < 0)
		DOT_N_R = -DOT_N_R;
	else
		ft_swap(&WORLD_FACT, &OBJECT_FACT);
	SWITCH_FACT = WORLD_FACT / OBJECT_FACT;
	INTERNAL_REFRACTION = 1 - SWITCH_FACT * SWITCH_FACT *
		(1 - DOT_N_R * DOT_N_R);
	if (INTERNAL_REFRACTION < 0)
		return ((t_vector){0, 0, 0});
	SCALED_RAY = ft_scale_vector(rtv.cam.ray_direction, SWITCH_FACT);
	REFRACTED_RAY = ft_scale_vector(rtv.cam.hit.normal,
		(SWITCH_FACT * DOT_N_R - sqrt(INTERNAL_REFRACTION)));
	RESULT = ft_add_vector(SCALED_RAY, REFRACTED_RAY);
	return (ft_normalise_vector(RESULT));
}

t_color		ft_refract_ray(t_rtv rtv, int depth)
{
	if (depth > rtv.scene.refraction_depth || !rtv.options.refraction)
		return ((t_color){0, 0, 0});
	rtv.cam.ray_direction = ft_get_refracted_ray(rtv);
	rtv.cam.ray_origin = ft_add_vector(rtv.cam.hit.position,
		ft_scale_vector(rtv.cam.ray_direction, MIN_D));
	if (ft_intersect_refracted(&rtv))
		return (ft_get_node_color(rtv, depth + 1));
	else
		return ((t_color){0, 0, 0});
}
