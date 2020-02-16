/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors_calcul.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/26 07:26:02 by abenaiss          #+#    #+#             */
/*   Updated: 2020/02/15 05:07:09 by abenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_color			ft_diffuse(t_light light, t_vector light_vect,
	t_vector normal, t_color color)
{
	t_color		rgb;
	double	ln;

	ln =  ft_clip_min(0, ft_dot_vector(light_vect, normal));
	rgb.r = ln * color.r * light.color.r * light.intensity;
	rgb.g = ln * color.g * light.color.g * light.intensity;
	rgb.b = ln * color.b * light.color.b * light.intensity;
	return (rgb);
}

t_color			ft_specular(t_light light, t_vector normal,
	t_vector reflected_light_vect)
{
	t_color rgb;
	double	alpha;
	double ln;

	alpha = 40;
	ln = pow(ft_clip_min(0, ft_dot_vector(normal, reflected_light_vect)), alpha);
	rgb.r = ln * (light.color.r * light.intensity);
	rgb.g = ln * (light.color.g * light.intensity);
	rgb.b = ln * (light.color.b * light.intensity);
	return (rgb);
}

t_color			ft_cartoon_diffuse(t_light light, t_vector light_vect,
	t_vector normal, t_color color)
{
	t_color		rgb;
	double	ln;

	ln =  ft_clip_min(0, ft_dot_vector(light_vect, normal));
	ln = (double)((int)((ln * 10) / 5) * 5)/10;
	rgb.r = ln * color.r * light.color.r * light.intensity;
	rgb.g = ln * color.g * light.color.g * light.intensity;
	rgb.b = ln * color.b * light.color.b * light.intensity;
	return (rgb);
}

t_color			ft_cartoon_specular(t_light light, t_vector normal,
	t_vector reflected_light_vect)
{
	t_color rgb;
	double	alpha;
	double ln;

	alpha = 40;
	ln = pow(ft_clip_min(0, ft_dot_vector(normal, reflected_light_vect)), alpha);
	ln = (double)((int)((ln * 10) / 5) * 5)/10;
	rgb.r = ln * (light.color.r * light.intensity);
	rgb.g = ln * (light.color.g * light.intensity);
	rgb.b = ln * (light.color.b * light.intensity);
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
	// if (shadow_ratio > -1)
		return (shadow_ratio);
	return (0);
}
