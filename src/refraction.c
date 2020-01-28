/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   refraction.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiri <abiri@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/11 20:48:50 by abiri             #+#    #+#             */
/*   Updated: 2020/01/28 18:56:08 by abiri            ###   ########.fr       */
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

int		ft_intersect_refracted(t_rtv *rtv, t_object *current_object)
{
	t_object_list	*node;
	double			temp_min;
	double			min;

	node = rtv->objects;
	min = MAX_D;
	while (node)
	{
		if (&node->object != current_object)
		{
			if (node->object.point.reflection == 1)
				ft_choose_intersection(node, rtv, &min);
		}
		node = node->next;
	}
	if (min != MAX_D)
		return (1);
	return (0);
}

t_vector	ft_get_refracted_ray(t_rtv rtv)
{
	t_vector	Nrefr;
	double	n1, n2;
	double	NdotI;

	n1 = 1;
	n2 = rtv.cam.hit.refraction;
	NdotI = ft_dot_vector(rtv.cam.hit.normal, rtv.cam.ray_direction);
	Nrefr = rtv.cam.hit.normal;
	if (NdotI < 0)
		NdotI = -NdotI;
	else
	{
		Nrefr = ft_scale_vector(Nrefr, -1);
		ft_swap(&n1, &n2);
	}
	double	nn;

	nn = n1 / n2;
	double cosi;

	double	k = 1 - nn * nn * (1 - NdotI * NdotI);
	if (k < 0)
		return ((t_vector){0, 0, 0});
	t_vector	t1;
	t_vector	t2;
	t_vector	result;

	t1 = ft_scale_vector(rtv.cam.ray_direction, nn);
	t2 = ft_scale_vector(rtv.cam.hit.normal, (nn * NdotI - sqrt(k)));
	result = ft_add_vector(t1, t2);
	return (ft_normalise_vector(result));
}

t_color	ft_refract_ray(t_rtv rtv, int depth)
{
	t_vector	Nrefr;
	double	n1, n2;
	double	NdotI;

	n1 = 1;
	n2 = rtv.cam.hit.refraction;
	if (depth >= rtv.scene.refraction_depth)
		return ((t_color){0, 0, 0});
	NdotI = ft_dot_vector(rtv.cam.hit.normal, rtv.cam.ray_direction);
	Nrefr = rtv.cam.hit.normal;
	if (NdotI < 0)
		NdotI = -NdotI;
	else
	{
		Nrefr = ft_scale_vector(Nrefr, -1);
		ft_swap(&n1, &n2);
	}
	double	nn;

	nn = n1 / n2;
	double cosi;

	double	k = 1 - nn * nn * (1 - NdotI * NdotI);
	if (k < 0)
		return ((t_color){0, 0, 0});
	t_vector	t1;
	t_vector	t2;
	t_vector	result;

	t1 = ft_scale_vector(rtv.cam.ray_direction, nn);
	t2 = ft_scale_vector(rtv.cam.hit.normal, (nn * NdotI - sqrt(k)));
	result = ft_add_vector(t1, t2);
	rtv.cam.ray_direction = ft_normalise_vector(result);
	rtv.cam.ray_origin = rtv.cam.hit.position;
	//t_color	mycolor = ft_reflect_ray(rtv, color);
	if (ft_intersect_refracted(&rtv, rtv.cam.hit.object))
		return (ft_get_node_color(rtv, depth + 1));
	else
		return ((t_color){0, 0, 0});
}
