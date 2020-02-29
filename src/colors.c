/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiri <abiri@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 22:04:25 by azarzor           #+#    #+#             */
/*   Updated: 2020/02/23 03:47:32 by abiri            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

double				ft_get_mapped_specular(t_rtv *rtv)
{
	t_color			specular;

	specular = ft_get_texture_color(
			rtv->cam.hit.object->point.material.specular,
			rtv->cam.hit.uv, (t_color){1, 1, 1},
			rtv->cam.hit.object->point.material.mode);
	return (specular.r);
}

t_color				ft_shader(t_light light,
	t_rtv *rtv, t_color color, t_vector normal)
{
	t_vector		light_vect[2];
	t_color			shader[2];

	DIFFUSE = (t_color){0, 0, 0};
	SPECULAR = (t_color){0, 0, 0};
	LIGHT_VECTOR = (light.light_type == SPOT_LIGHT) ?
	ft_scale_vector(light.light_direction, -1) : ft_normalise_vector(
	ft_sub_vector(light.center, rtv->cam.hit.position));
	REFLECTED_LIGHT_VECTOR = ft_reflected_light_ray(LIGHT_VECTOR, normal);
	ft_check_shadow(*rtv, &light, LIGHT_VECTOR);
	if (rtv->options.diffuse && rtv->scene.effect != 4)
		DIFFUSE = ft_diffuse(light, LIGHT_VECTOR, normal, color);
	if (rtv->options.specular && rtv->scene.effect != 4)
		SPECULAR = ft_specular(light,
	rtv->cam.ray_direction, REFLECTED_LIGHT_VECTOR);
	if (rtv->options.diffuse && rtv->scene.effect == 4)
		DIFFUSE = ft_cartoon_diffuse(light, LIGHT_VECTOR, normal, color);
	if (rtv->options.specular && rtv->scene.effect == 4)
		SPECULAR = ft_cartoon_specular(light,
	rtv->cam.ray_direction, REFLECTED_LIGHT_VECTOR);
	if (rtv->cam.hit.object->point.material.specular)
		SPECULAR = ft_scale_colors(SPECULAR,
			ft_get_mapped_specular(rtv));
	return (ft_add_colors(SPECULAR, DIFFUSE));
}

static void			ft_random_lights(t_light *light,
	t_vector center, double radius, int index)
{
	t_vector teta;

	if (index)
	{
		teta.x = ((double)rand() / (double)RAND_MAX) * (100.0 * M_PI);
		teta.y = ((double)rand() / (double)RAND_MAX) * (100.0 * M_PI);
		teta.z = ((double)rand() / (double)RAND_MAX) * (100.0 * M_PI);
		light->center.x = center.x + radius * cos(teta.x);
		light->center.y = center.y + radius * cos(teta.y);
		light->center.z = center.z + radius * sin(teta.z);
	}
}

t_color				ft_mix_colors(t_rtv *rtv, t_vector normal, t_color color)
{
	t_color			col;
	t_light_list	*light_node;
	t_light			light;
	t_vector		center;
	int				i;

	col = (t_color){0, 0, 0};
	light_node = rtv->lights;
	while (light_node)
	{
		light = light_node->light;
		center = light.center;
		i = -1;
		while (++i < ((rtv->options.soft_shadows) ?
			rtv->scene.light_samples : 1))
		{
			light.color = light_node->light.color;
			light.intensity = light_node->light.intensity /
				((rtv->options.soft_shadows) ? rtv->scene.light_samples : 1);
			ft_random_lights(&light, center, light.radius, i);
			col = ft_add_colors(col, ft_shader(light, rtv, color, normal));
		}
		light_node = light_node->next;
	}
	return (ft_add_colors(col, ft_scale_colors(color, rtv->scene.ambiant)));
}

t_vector			ft_reflected_light_ray(t_vector light_vect, t_vector normal)
{
	t_vector	scaled_normal;
	double		normal_scalar;

	normal_scalar = 2.0 * ft_dot_vector(normal, light_vect);
	scaled_normal = ft_scale_vector(normal, normal_scalar);
	return (ft_normalise_vector(ft_sub_vector(
		light_vect, scaled_normal)));
}
