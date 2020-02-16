/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 22:04:25 by azarzor           #+#    #+#             */
/*   Updated: 2020/02/15 05:21:07 by abenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#define LIGHT_SPHERE_COUNT 1.0

#include "rtv1.h"

#define LIGHT_NUMBER 50
t_vector random_unit_vector(double radius, t_vector center)
{   t_vector v;
    double theta = ((2.0 * M_PI)) * ( (double)rand() / (double)RAND_MAX );
    double x = ((2.0)) * ( (double)rand() / (double)RAND_MAX ) + 2.0 - 1.0;
    double s = sqrt(1.0 + x * x);
    v.x = x;
    v.y = s * cos(theta);
    v.z = s * sin(theta);
	return(ft_add_vector(center, ft_scale_vector(ft_normalise_vector(v), radius)));
}

double			ft_check_shadow(t_rtv rtv, t_light light,
					t_vector light_vect, t_color *color)
{
	double intersection_dist;
	double dot;

	rtv.cam.position = rtv.cam.hit.position;
	rtv.cam.ray_direction = light_vect;
	intersection_dist = ft_check_intersection(rtv);
	dot = ft_dot_vector(light_vect, rtv.cam.hit.normal);
	// if (intersection_dist)
	// {
	// 	if ((intersection_dist > ft_vector_size(ft_sub_vector(light.center
	// 		, rtv.cam.hit.position))))
	// 		return (1);
	// 	if (dot >= 0)
	// 		return (0);
	// 	else
	// 	{
	// 		*color = ft_add_colors(*color, ft_scale_colors(*color,
	// 		ft_calculate_shadow(rtv, intersection_dist, light, light_vect)));
	// 		return (0);
	// 	}
	// }
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
		REFLECTED_LIGHT_VECTOR = ft_reflected_light_ray(LIGHT_VECTOR, normal);
		if (ft_check_shadow(*rtv, light_node->light, LIGHT_VECTOR, &color))
		{
			dif_col = ft_add_colors(dif_col,
				(rtv->scene.effect != 5) ? ft_diffuse(light_node->light, LIGHT_VECTOR, normal, color)
					: ft_cartoon_diffuse(light_node->light, LIGHT_VECTOR, normal, color));
			spec_col = ft_add_colors(spec_col,
			(rtv->scene.effect != 5) ? ft_specular(light_node->light, rtv->cam.ray_direction, REFLECTED_LIGHT_VECTOR)
			: ft_cartoon_specular(light_node->light, rtv->cam.ray_direction, REFLECTED_LIGHT_VECTOR));
		}
		light_node = light_node->next;
	}
	return (ft_add_colors(dif_col, ft_add_colors(spec_col,
		ft_scale_colors(color, rtv->scene.ambiant))));
}

t_vector		ft_reflected_light_ray(t_vector light_vect, t_vector normal)
{
	t_vector	scaled_normal;
	double		normal_scalar;

	normal_scalar = 2.0 * ft_dot_vector(normal, light_vect);
	scaled_normal = ft_scale_vector(normal, normal_scalar);
	return (ft_normalise_vector(ft_sub_vector(
		light_vect, scaled_normal)));
}
