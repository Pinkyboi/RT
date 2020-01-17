/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normal_and_refraction.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/19 18:58:29 by abenaiss          #+#    #+#             */
/*   Updated: 2020/01/16 23:02:11 by abenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void	ft_get_hit_info(t_vector normal, t_point *point, t_cam *cam)
{
	if (cam->hit.soluces[0])
	{
		cam->hit.normal = normal;
		cam->hit.color = point->color;
		cam->hit.reflection = point->reflection;
		cam->hit.refraction = point->refraction;
		cam->hit.transparency = point->transparency;
	}
}

void	ft_sphere_normal(t_cam *cam, t_sphere *sphere, double distance)
{
	t_vector	radius;
	t_vector	normal;

	ft_intersection_position(cam, distance);
	cam->hit.soluces[0] = ft_sphere_limit(*sphere, *cam);
	radius = ft_sub_vector(cam->hit.position, sphere->center);
	normal = ft_normalise_vector(radius);
	ft_get_hit_info(normal, (t_point *)sphere, cam);
}

void	ft_cylinder_normal(t_cam *cam, t_cylinder *cylinder,
	double distance)
{
	t_vector	center_to_point;
	t_vector	scaled_axis;
	double		scale;
	t_vector	normal;

	ft_intersection_position(cam, distance);
	cylinder->axis = ft_normalise_vector(cylinder->axis);
	scale = ft_dot_vector(cam->ray_direction, cylinder->axis) * distance;
	scale += ft_dot_vector(ft_sub_vector(cam->position,
				cylinder->center), cylinder->axis);
	cylinder->lenght = scale;
	cam->hit.soluces[0] = ft_cylinder_limit(*cylinder, *cam);
	center_to_point = ft_sub_vector(cam->hit.position, cylinder->center);
	scaled_axis = ft_scale_vector(cylinder->axis, scale);
	normal = ft_normalise_vector(ft_sub_vector(center_to_point,
					scaled_axis));
	ft_get_hit_info(normal, (t_point *)cylinder, cam);
}

void	ft_cone_normal(t_cam *cam, t_cone *cone, double distance)
{
	t_vector	center_to_point;
	t_vector	scaled_axis;
	double		scale;
	t_vector	normal;

	ft_intersection_position(cam, distance);
	cam->hit.soluces[0] = ft_cone_limit(*cone, *cam);
	cone->axis = ft_normalise_vector(cone->axis);
	scale = ft_dot_vector(cam->ray_direction, cone->axis) * distance;
	scale += ft_dot_vector(ft_sub_vector(cam->position,
				cone->center), cone->axis);
	center_to_point = ft_sub_vector(cam->hit.position, cone->center);
	scaled_axis = ft_scale_vector(ft_scale_vector(cone->axis, scale),
								(1 + FT_SQR(cone->tilt)));
	normal = ft_normalise_vector(ft_sub_vector(center_to_point,
					scaled_axis));
	ft_get_hit_info(normal, (t_point *)cone, cam);
}

void	ft_reflected_light_ray(t_cam cam, t_light *light, t_vector normal)
{
	t_vector	scaled_normal;
	t_vector	light_vect;
	double		normal_scalar;

	light->light_vect = ft_normalise_vector(ft_sub_vector(light->center,
				cam.hit.position));
	normal_scalar = 2.0 * ft_dot_vector(normal, light->light_vect);
	scaled_normal = ft_scale_vector(normal, normal_scalar);
	light->reflected_light_vect = ft_normalise_vector(ft_sub_vector(
		scaled_normal, light->light_vect));
}
