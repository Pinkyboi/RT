/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiri <abiri@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 22:04:25 by azarzor           #+#    #+#             */
/*   Updated: 2020/01/27 20:26:54 by abiri            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

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

	if (depth > rtv.scene.refraction_depth)
		return (light);
	shadow_object = ft_get_intersection_object(&rtv, &dist);
	this_light.intensity = light.intensity;
	this_light.color = light.color;
	if (shadow_object && (dist < ft_vector_size(ft_sub_vector(rtv.cam.position, light.center))))
	{
			this_light.intensity *= shadow_object->point.transparency;
			this_light.color = ft_merge_color(this_light.color, rtv.cam.hit.color);
			rtv.cam.position = rtv.cam.hit.position;
			rtv.cam.ray_direction = ft_get_refracted_ray(rtv);
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

	rtv.cam.position = rtv.cam.hit.position;
	rtv.cam.ray_direction = light_vec;
	this_light = ft_get_shadow_light(rtv, *light, light_vec, 1);
	light->color = this_light.color;
	light->intensity = this_light.intensity;
	return (this_light.intensity);
	//shadow_object = NULL;
	//shadow_object = ft_get_intersection_object(&rtv, &dist);
	/*if (shadow_object && (dist < ft_vector_size(ft_sub_vector(rtv.cam.position, light.center))))
		return (0);*/
}

/*double			ft_check_shadow(t_rtv rtv, t_light light,
					t_vector light_vect, t_color *color)
{
	double intersection_dist;
	double dot;

	rtv.cam.position = rtv.cam.hit.position;
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
}*/

t_color			ft_mix_colors(t_rtv *rtv, t_vector normal, t_color color)
{
	t_color			dif_col;
	t_color			spec_col;
	t_light_list	*light_node;
	t_vector		light_vect[2];
	double			shadow_ratio;
	t_light			light;
	t_color			new_color;

	dif_col = (t_color){0, 0, 0};
	spec_col = (t_color){0, 0, 0};
	light_node = rtv->lights;
	while (light_node)
	{
		LIGHT_VECTOR = ft_normalise_vector(
			ft_sub_vector(light_node->light.center, rtv->cam.hit.position));
		REFLECTED_LIGHT_VECTOR = ft_reflected_light_ray(
			rtv->cam, &light_node->light, LIGHT_VECTOR, normal);
		new_color = color;
		light = light_node->light;
		ft_check_shadow(*rtv, &light, LIGHT_VECTOR, &new_color);
		dif_col = ft_add_colors(dif_col,
			ft_diffuse(light, LIGHT_VECTOR, normal, color));
		spec_col = ft_add_colors(spec_col,
			ft_specular(light, normal, REFLECTED_LIGHT_VECTOR));
//		dif_col = ft_scale_colors(dif_col, shadow_ratio);
//		spec_col = ft_scale_colors(spec_col, shadow_ratio);
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
