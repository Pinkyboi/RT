/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   refraction.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiri <abiri@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/11 20:48:50 by abiri             #+#    #+#             */
/*   Updated: 2020/02/07 19:28:01 by abiri            ###   ########.fr       */
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
		if (node->object.point.reflection == 1)
			ft_choose_intersection(node, rtv, &min);
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
		ft_swap(&n1, &n2);
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
	if (depth > rtv.scene.refraction_depth)
		return ((t_color){0, 0, 0});
	rtv.cam.ray_direction = ft_get_refracted_ray(rtv);
	rtv.cam.ray_origin = rtv.cam.hit.position;

	if (ft_intersect_refracted(&rtv, rtv.cam.hit.object))
		return (ft_get_node_color(rtv, depth + 1));
	else
		return ((t_color){0, 0, 0});
}
