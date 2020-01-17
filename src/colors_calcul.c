/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors_calcul.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/26 07:26:02 by abenaiss          #+#    #+#             */
/*   Updated: 2020/01/18 00:01:49 by abenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_color			ft_diffuse(t_light light, t_vector light_vect,
	t_vector normal, t_color color)
{
	t_color		rgb;

	rgb.r = ft_clip_min_max(0, 1, ft_dot_vector(normal, light_vect))
		* color.r * light.color.r * light.intensity;
	rgb.g = ft_clip_min_max(0, 1, ft_dot_vector(normal, light_vect))
		* color.g * light.color.g * light.intensity;
	rgb.b = ft_clip_min_max(0, 1, ft_dot_vector(normal, light_vect))
		* color.b * light.color.b * light.intensity;
	return (rgb);
}

t_color			ft_specular(t_light light, t_vector normal,
	t_vector reflected_light_vect)
{
	t_color rgb;
	double	alpha;

	alpha = 100;
	rgb.r = pow(ft_clip_min(0, ft_dot_vector(normal,
					reflected_light_vect)), alpha)
					* (light.color.r * light.intensity);
	rgb.g = pow(ft_clip_min(0, ft_dot_vector(normal,
					reflected_light_vect)), alpha)
					* (light.color.g * light.intensity);
	rgb.b = pow(ft_clip_min(0, ft_dot_vector(normal,
					reflected_light_vect)), alpha)
					* (light.color.b * light.intensity);
	return (rgb);
}

double			ft_calculate_shadow(t_rtv rtv, double intersection_dist,
					t_light light, t_vector light_vect)
{
	double	light_distance;
	double	intersection_distance;
	double	shadow_ratio;

	light_distance = ft_vector_size(ft_sub_vector(light.center,
		rtv.cam.hit.position));
	intersection_distance = ft_vector_size(ft_add_vector(
		ft_scale_vector(light_vect, intersection_dist),
		rtv.cam.hit.position));
	shadow_ratio = -(intersection_distance / light_distance);
	if (shadow_ratio > -1)
		return (shadow_ratio);
	return (0);
}
