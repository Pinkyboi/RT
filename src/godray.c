/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   godray.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiri <abiri@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/10 22:23:53 by abiri             #+#    #+#             */
/*   Updated: 2020/02/11 00:02:30 by abiri            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#define GODRAY_STEP 0.1
#define GODRAY_MAX_STEPS 50
#define GODRAY_PARTICLE_COLOR (t_color){1, 1, 1}
#define GODRAY_PARTICLE_TRANSPARENCY 0.001

double	ft_check_position_illumination(t_vector pos, t_rtv rtv)
{
	double			light_intensity;
	double			final_result;
	t_vector		light_vec;
	t_light_list	*light;
	t_light			temp_light;

	light = rtv.lights;
	final_result = 0;
	rtv.cam.hit.position = pos;
	while (light)
	{
		light_vec = ft_normalise_vector(ft_sub_vector(light->light.center, pos));
		temp_light = light->light;
		light_intensity = ft_check_shadow(rtv, &temp_light, light_vec, NULL);
		final_result += light_intensity *  GODRAY_PARTICLE_TRANSPARENCY;
		light = light->next;
	}
	return (final_result);
}

t_color	ft_godray_effect(t_rtv *rtv)
{
	t_color		result;
	t_vector	pos;
	t_vector	iter;
	double		distance;

	iter = ft_scale_vector(rtv->cam.ray_direction, GODRAY_STEP);
	pos = rtv->cam.ray_origin;
	distance = GODRAY_STEP;
	result = (t_color){0, 0, 0};
	while (distance < GODRAY_MAX_STEPS && distance < rtv->cam.hit.soluces[0])
	{
		pos = ft_add_vector(pos, iter);
		result = ft_add_colors(result, ft_scale_colors(GODRAY_PARTICLE_COLOR,
			ft_check_position_illumination(pos, *rtv)));
		distance += GODRAY_STEP;
	}
	return (result);
}