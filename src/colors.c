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

t_object		*ft_get_intersection_object(t_rtv *rtv, double *min)
{
	t_object_list	*object;
	t_object		*best_object;

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

t_light			ft_get_shadow_light(t_rtv rtv, t_light light,
	t_vector light_vec, int depth)
{
	t_light		this_light;
	t_object	*shadow_object;
	double		dist;

	if (depth > 1 &&
		(depth > rtv.scene.refraction_depth + 1 || !rtv.options.refraction))
		return (light);
	shadow_object = ft_get_intersection_object(&rtv, &dist);
	this_light.intensity = light.intensity;
	this_light.color = light.color;
	if (shadow_object && (dist < ft_vector_size(
		ft_sub_vector(rtv.cam.position, light.center))))
	{
		this_light.intensity *= rtv.cam.hit.transparency;
		this_light.color = ft_merge_color(this_light.color, rtv.cam.hit.color);
		rtv.cam.ray_origin = rtv.cam.hit.position;
		rtv.cam.position = rtv.cam.hit.position;
		rtv.cam.ray_direction = ft_get_refracted_ray(rtv);
		return ((this_light.intensity == 0) ? this_light :
			ft_get_shadow_light(rtv, this_light, light_vec, depth + 1));
	}
	else
		light.intensity *= ft_clip_min(0,
			ft_dot_vector(ft_normalise_vector(light_vec),
				ft_normalise_vector(rtv.cam.ray_direction)));
	return (light);
}

double			ft_basic_sphere_intersection(t_cam *cam,
		t_sphere *sphere, double min)
{
	double	abc[3];
	double	delta;

	A = 1;
	B = 2 * ft_dot_vector(cam->ray_direction,
			ft_sub_vector(cam->ray_origin, sphere->center));
	C = FT_SQR(ft_vector_size(ft_sub_vector(cam->ray_origin, sphere->center)))
		- (sphere->radius * sphere->radius);
	delta = (B * B) - (4 * A * C);
	if (delta < 0)
		return (0);
	return (1);
}

double			ft_check_shadow(t_rtv rtv, t_light *light,
	t_vector light_vec, t_color *color)
{
	t_light	this_light;

	(void)*color;
	rtv.cam.ray_origin = ft_add_vector(
		ft_scale_vector(rtv.cam.hit.normal, MIN_D), rtv.cam.hit.position);
	rtv.cam.position = rtv.cam.ray_origin;
	rtv.cam.ray_direction = light_vec;
	this_light = ft_get_shadow_light(rtv, *light, light_vec, 1);
	if (!(ft_basic_sphere_intersection(&(rtv.cam),
		&(light->light_shape), MAX_D)))
		this_light.intensity = 0;
	light->intensity = this_light.intensity;
	return (this_light.intensity);
	/*
	* NO IDEA WHAT THIS IS
	*t_object	*shadow_object;
	*light->color = this_light.color;
	*shadow_object = NULL;
	*shadow_object = ft_get_intersection_object(&rtv, &dist);
	*if (shadow_object && (dist < f
	*t_vector_size(ft_sub_vector(rtv.cam.position, light.center))))
	*	return (0);*/
}

t_vector		ft_random_lights(t_light light,
	t_vector center, double radius)
{
	t_vector teta;

	teta.x = ((double)rand() / (double)RAND_MAX) * (100.0 * M_PI);
	teta.y = ((double)rand() / (double)RAND_MAX) * (100.0 * M_PI);
	teta.z = ((double)rand() / (double)RAND_MAX) * (100.0 * M_PI);
	light.center.x = center.x + radius * cos(teta.x);
	light.center.y = center.y + radius * cos(teta.y);
	light.center.z = center.z + radius * sin(teta.z);
	return (light.center);
}

t_color			ft_choose_shader(t_light light,
	t_rtv *rtv, t_color color, t_vector normal)
{
	t_vector		light_vect[2];
	t_color			new_color;
	t_color			shader[2];

	DIFFUSE = (t_color){0, 0, 0};
	SPECULAR = (t_color){0, 0, 0};
	LIGHT_VECTOR = (light.light_type == SPOT_LIGHT) ?
	ft_scale_vector(light.light_direction, -1) : ft_normalise_vector(
	ft_sub_vector(light.center, rtv->cam.hit.position));
	REFLECTED_LIGHT_VECTOR = ft_reflected_light_ray(LIGHT_VECTOR, normal);
	ft_check_shadow(*rtv, &light, LIGHT_VECTOR, &new_color);
	if (rtv->options.diffuse)
		DIFFUSE = ft_diffuse(light, LIGHT_VECTOR, normal, color);
	if (rtv->options.specular)
		SPECULAR = ft_specular(light,
	rtv->cam.ray_direction, REFLECTED_LIGHT_VECTOR);
	if (rtv->cam.hit.object->point.material.specular)
	{
		new_color = ft_get_texture_color(
			rtv->cam.hit.object->point.material.specular,
			rtv->cam.hit.uv, (t_color){1, 1, 1},
			rtv->cam.hit.object->point.material.mode);
		SPECULAR = ft_scale_colors(SPECULAR, new_color.r);
	}
	return (ft_add_colors(SPECULAR, DIFFUSE));
}

t_color			ft_mix_colors(t_rtv *rtv, t_vector normal, t_color color)
{
	t_color			shader;
	t_light_list	*light_node;
	t_light			light;
	t_vector		center;
	int				i;

	shader = (t_color){0, 0, 0};
	light_node = rtv->lights;
	while (light_node)
	{
		light = light_node->light;
		center = light.center;
		i = -1;
		while (++i < ((rtv->options.soft_shadows) ? rtv->scene.light_samples : 1))
		{
			light.color = light_node->light.color;
			light.intensity = light_node->light.intensity /
				((rtv->options.soft_shadows) ? rtv->scene.light_samples : 1);
			if (i != 0)
				light.center = ft_random_lights(light, center, light.radius);
			shader = ft_add_colors(shader,
			ft_choose_shader(light, rtv, color, normal));
		}
		light_node = light_node->next;
	}
	return (ft_add_colors(shader, ft_scale_colors(color, rtv->scene.ambiant)));
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
