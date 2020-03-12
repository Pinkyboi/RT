/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadows.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azarzor <azarzor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 23:36:40 by abenaiss          #+#    #+#             */
/*   Updated: 2020/03/04 21:35:01 by azarzor          ###   ########.fr       */
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
	this_light = light;
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
		t_sphere *sphere)
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
	t_vector light_vec)
{
	t_light	this_light;

	rtv.cam.ray_origin = ft_add_vector(
		ft_scale_vector(rtv.cam.hit.normal, 0.00001), rtv.cam.hit.position);
	rtv.cam.position = rtv.cam.ray_origin;
	rtv.cam.ray_direction = light_vec;
	this_light = ft_get_shadow_light(rtv, *light, light_vec, 1);
	if (!(ft_basic_sphere_intersection(&(rtv.cam),
		&(light->light_shape))))
		this_light.intensity = 0;
	light->intensity = this_light.intensity;
	return (this_light.intensity);
}
