/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiri <abiri@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 22:04:25 by azarzor           #+#    #+#             */
/*   Updated: 2020/02/07 19:18:40 by abiri            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#define LIGHT_SPHERE_COUNT 1.0

t_object		*ft_get_intersection_object(t_rtv *rtv, double *min)
{
	t_object_list *object;
	t_object *best_object;

	object = rtv->objects;
	*min = MAX_D;
	best_object = NULL;
	while (object)
	{
		if (ft_choose_intersection(object, rtv, min))
			best_object = &object->object;
		object = object->next;
	}
	return (best_object);
}

t_color			ft_merge_color(t_color first, t_color second)
{
	t_color result;

	result.r = (first.r < second.r) ? first.r : second.r;
	result.g = (first.g < second.g) ? first.g : second.g;
	result.b = (first.b < second.b) ? first.b : second.b;
	return (result);
}

t_light			ft_get_shadow_light(t_rtv rtv, t_light light, t_vector light_vec, int depth)
{
	t_light		this_light;
	t_object	*shadow_object;
	double		dist;

	if (depth > rtv.scene.refraction_depth + 1)
		return (light);
	shadow_object = ft_get_intersection_object(&rtv, &dist);
	this_light.intensity = light.intensity;
	this_light.color = light.color;
	if (shadow_object && (dist < ft_vector_size(ft_sub_vector(rtv.cam.position, light.center))))
	{
		this_light.intensity *= rtv.cam.hit.transparency;
		this_light.color = ft_merge_color(this_light.color, rtv.cam.hit.color);
		rtv.cam.ray_origin = rtv.cam.hit.position;
		rtv.cam.position = rtv.cam.hit.position;
		rtv.cam.ray_direction = ft_get_refracted_ray(rtv);
		if (this_light.intensity == 0)
			return (this_light);
		return (ft_get_shadow_light(rtv, this_light, light_vec, depth + 1));
	}
	else
		light.intensity *= ft_clip_min(0,
			ft_dot_vector(ft_normalise_vector(light_vec), ft_normalise_vector(rtv.cam.ray_direction)));
	return (light);
}

double	ft_check_shadow(t_rtv rtv, t_light *light, t_vector light_vec, t_color *color)
{
	//t_object	*shadow_object;
	t_light	this_light;
	double		dist;

	rtv.cam.ray_origin = rtv.cam.hit.position;
	rtv.cam.position = rtv.cam.hit.position;
	rtv.cam.ray_direction = light_vec;
	this_light = ft_get_shadow_light(rtv, *light, light_vec, 1);
	//light->color = this_light.color;
	light->intensity = this_light.intensity;
	return (this_light.intensity);
	//shadow_object = NULL;
	//shadow_object = ft_get_intersection_object(&rtv, &dist);
	//if (shadow_object && (dist < ft_vector_size(ft_sub_vector(rtv.cam.position, light.center))))
	//	return (0);
}

t_vector random_point_sphere(double radius, t_vector center)
{   t_vector v;

    double theta = ((2.0 * M_PI)) * ( (double)rand() / (double)RAND_MAX );
    double x = ((2.0)) * ( (double)rand() / (double)RAND_MAX ) + 2.0 - 1.0;
    double s = sqrt(fabs(1.0 - x * x));
    v.x = x;
    v.y = s * cos(theta);
    v.z = s * sin(theta);
    return(ft_add_vector(center, ft_scale_vector(ft_normalise_vector(v), radius)));
}

t_color			ft_mix_colors(t_rtv *rtv, t_vector normal, t_color color)
{
	t_color			dif_col;
	t_color			spec_col;
	t_light_list	*light_node;
	t_vector		light_vect[2];
	double			shadow_ratio;
	t_light			light;
	t_color			new_color;
	double	tetax;
	double	tetay;
	double	tetaz;
	t_vector	center;
	double	radius;

	dif_col = (t_color){0, 0, 0};
	spec_col = (t_color){0, 0, 0};
	light_node = rtv->lights;
	while (light_node)
	{
		light = light_node->light;
		center = light.center;
		radius = 0;
		for (int i = 0; i < LIGHT_SPHERE_COUNT; i++)
		{
			light.color = light_node->light.color;
			light.intensity = light_node->light.intensity / LIGHT_SPHERE_COUNT;
			tetax = ((double)rand() / (double)RAND_MAX) * (100.0 * M_PI);
			tetay = ((double)rand() / (double)RAND_MAX) * (100.0 * M_PI);
			tetaz = ((double)rand() / (double)RAND_MAX) * (100.0 * M_PI);
			light.center.x = center.x + radius * cos(tetax);
			light.center.y = center.y + radius * cos(tetay);
			light.center.z = center.z + radius * sin(tetaz);
			LIGHT_VECTOR = ft_normalise_vector(
				ft_sub_vector(light.center, rtv->cam.hit.position));
			REFLECTED_LIGHT_VECTOR = ft_reflected_light_ray(
				&light, LIGHT_VECTOR, normal);
			ft_check_shadow(*rtv, &light, LIGHT_VECTOR, &new_color);
			dif_col = ft_add_colors(dif_col,
				ft_diffuse(light, LIGHT_VECTOR, normal, color));
			spec_col = ft_add_colors(spec_col,
				ft_specular(light, normal, REFLECTED_LIGHT_VECTOR));
			if (rtv->cam.hit.object->point.material.specular)
			{
				new_color = ft_get_texture_color(rtv->cam.hit.object->point.material.specular,
					rtv->cam.hit.uv, (t_color){1, 1, 1}, rtv->cam.hit.object->point.material.mode);
				spec_col = ft_scale_colors(spec_col, new_color.r);
			}
		}
		light_node = light_node->next;
	}
	return (ft_add_colors(dif_col, ft_add_colors(spec_col,
		ft_scale_colors(color, rtv->scene.ambiant))));
}

t_vector		ft_reflected_light_ray(t_light *light,
	t_vector light_vect, t_vector normal)
{
	t_vector	scaled_normal;
	double		normal_scalar;

	normal_scalar = 2.0 * ft_dot_vector(normal, light_vect);
	scaled_normal = ft_scale_vector(normal, normal_scalar);
	return (ft_normalise_vector(ft_sub_vector(
		scaled_normal, light_vect)));
}
