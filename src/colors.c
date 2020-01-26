/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azarzor <azarzor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 22:04:25 by azarzor           #+#    #+#             */
/*   Updated: 2020/01/21 15:22:41 by azarzor          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

double			ft_check_shadow(t_rtv rtv, t_light light,
					t_vector light_vect, t_color *color)
{
	double intersection_dist;
	double dot;

	rtv.cam.ray_origin = rtv.cam.hit.position;
	rtv.cam.ray_direction = light_vect;
	intersection_dist = ft_check_intersection(rtv);
	dot = ft_dot_vector(light_vect, rtv.cam.hit.normal);
	if (intersection_dist)
	{
		if ((intersection_dist > ft_vector_size(ft_sub_vector(light.center
			, rtv.cam.hit.position))) || dot < 0)
			return (1);
		if (dot >= 0)
			return (0);
		else
		{
			*color = ft_add_colors(*color, ft_scale_colors(*color,
			ft_calculate_shadow(rtv, intersection_dist, light, light_vect)));
			return (0);
		}
	}
	return (1);
}

t_color			ft_mix_colors(t_rtv *rtv, t_vector normal, t_color color)
{
	t_color			dif_col;
	t_color			spec_col;
	t_light_list	*light_node;
	t_vector		light_vect[2];

	dif_col = (t_color){0, 0, 0};
	spec_col = (t_color){0, 0, 0};
	light_node = rtv->lights;
	while (light_node)
	{
		LIGHT_VECTOR = ft_normalise_vector(
			ft_sub_vector(light_node->light.center, rtv->cam.hit.position));
		REFLECTED_LIGHT_VECTOR = ft_reflected_light_ray(
			rtv->cam, &light_node->light, LIGHT_VECTOR, normal);
		if (ft_check_shadow(*rtv, light_node->light, LIGHT_VECTOR, &color))
		{
			dif_col = ft_add_colors(dif_col,
				ft_diffuse(light_node->light, LIGHT_VECTOR, normal, color));
			spec_col = ft_add_colors(spec_col,
			ft_specular(light_node->light, normal, REFLECTED_LIGHT_VECTOR));
		}
		light_node = light_node->next;
	}
	return (ft_add_colors(dif_col, ft_add_colors(spec_col,
		ft_scale_colors(color, rtv->scene.ambiant))));
}

t_vector		ft_reflected_light_ray(t_cam cam, t_light *light,
	t_vector light_vect, t_vector normal)
{
	t_vector	scaled_normal;
	double		normal_scalar;

	normal_scalar = 2.0 * ft_dot_vector(normal, light_vect);
	scaled_normal = ft_scale_vector(normal, normal_scalar);
	return (ft_normalise_vector(ft_sub_vector(
		scaled_normal, light_vect)));
}
