/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constructors2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiri <kerneloverseer@pm.me>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/05 21:59:48 by abiri             #+#    #+#             */
/*   Updated: 2019/12/05 22:00:08 by abiri            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int				ft_add_light(t_xml_tag *tag, t_rtv *env)
{
	t_light	light;
	int		status;

	status = 0;
	light.center = ft_parse_vector(ft_xml_get_value(tag, "center",
				"(0,0,0)"), &status);
	light.color = ft_parse_color(ft_xml_get_value(tag, "color",
				"(255, 255, 255)"), &status);
	light.intensity = ft_parse_float(ft_xml_get_value(tag, "intensity",
				"1"), &status);
	light.intensity = ft_clamp_min_max(0, 1, light.intensity);
	status &= ft_light_push(env, light);
	return (status);
}

int				ft_load_camera(t_xml_tag *tag, t_rtv *env)
{
	int			status;

	status = 1;
	env->cam.position = ft_parse_vector(ft_xml_get_value(tag, "position",
				"(0,0,0)"), &status);
	env->cam.look_at = ft_parse_vector(ft_xml_get_value(tag, "lookat",
				"(0,0,-1)"), &status);
	env->cam.fov = ft_parse_float(ft_xml_get_value(tag, "fov",
				"35"), &status);
	env->cam.fov = ft_clamp_min(1, env->cam.fov);
	env->cam.translation = ft_parse_vector(ft_xml_get_value(tag, "translation",
				"(0,0,0)"), &status);
	return (status);
}
