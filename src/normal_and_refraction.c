/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normal_and_refraction.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/19 18:58:29 by abenaiss          #+#    #+#             */
/*   Updated: 2020/01/06 09:34:58 by abenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void	ft_sphere_normal(t_cam *cam, t_sphere *sphere, double distance)
{
	t_vector	radius;

	ft_intersection_position(cam, distance);
	ft_sphere_limit(sphere, *cam);
	radius = ft_sub_vector(cam->intersection, sphere->center);
	sphere->normal = ft_normalise_vector(radius);
}

void	ft_ellipsoid_normal(t_cam *cam, t_ellipsoid *ellipsoid, double distance)
{
	t_vector	normal;
	t_vector	radius;

	ft_intersection_position(cam, distance);
	radius = ft_sub_vector(cam->intersection, ellipsoid->center);
	normal.x = 2 * radius.x / FT_SQR(ellipsoid->axis.x);
	normal.y = 2 * radius.y / FT_SQR(ellipsoid->axis.y);
	normal.z = 2 * radius.z / FT_SQR(ellipsoid->axis.z);
	ellipsoid->normal = ft_normalise_vector(normal);
}

void	ft_hyperboloid_normal(t_cam *cam, t_hyperboloid *hyperboloid,
	double distance)
{
	t_vector	normal;
	t_vector	radius;

	ft_intersection_position(cam, distance);
	radius = ft_sub_vector(cam->intersection, hyperboloid->center);
	normal.x = 2 * radius.x;
	normal.y = -2 * radius.y;
	normal.z = 2 * radius.z;
	hyperboloid->normal = ft_normalise_vector(normal);
}

void	ft_paraboloid_normal(t_cam *cam, t_paraboloid *paraboloid,
	double distance)
{
	t_vector	normal;
	t_vector	radius;

	ft_intersection_position(cam, distance);
	radius = ft_sub_vector(cam->intersection, paraboloid->center);
	normal.x = 2 * radius.x;
	normal.y = -1;
	normal.z = 2 * radius.z;
	paraboloid->normal = ft_normalise_vector(normal);
}

void	ft_cylinder_normal(t_cam *cam, t_cylinder *cylinder,
	double distance)
{
	t_vector	center_to_point;
	t_vector	scaled_axis;
	double		scale;

	ft_intersection_position(cam, distance);
	cylinder->axis = ft_normalise_vector(cylinder->axis);
	scale = ft_dot_vector(cam->ray_direction, cylinder->axis) * distance;
	scale += ft_dot_vector(ft_sub_vector(cam->position,
				cylinder->center), cylinder->axis);
	cylinder->lenght = scale;
	ft_cylinder_limit(cylinder, *cam);
	center_to_point = ft_sub_vector(cam->intersection, cylinder->center);
	scaled_axis = ft_scale_vector(cylinder->axis, scale);
	cylinder->normal = ft_normalise_vector(ft_sub_vector(center_to_point,
				scaled_axis));
}

void	ft_cone_normal(t_cam *cam, t_cone *cone, double distance)
{
	t_vector	center_to_point;
	t_vector	scaled_axis;
	double		scale;

	ft_intersection_position(cam, distance);
	ft_cone_limit(cone, *cam);
	cone->axis = ft_normalise_vector(cone->axis);
	scale = ft_dot_vector(cam->ray_direction, cone->axis) * distance;
	scale += ft_dot_vector(ft_sub_vector(cam->position,
				cone->center), cone->axis);
	center_to_point = ft_sub_vector(cam->intersection, cone->center);
	scaled_axis = ft_scale_vector(ft_scale_vector(cone->axis, scale),
								(1 + FT_SQR(cone->tilt)));
	cone->normal = ft_normalise_vector(ft_sub_vector(center_to_point,
				scaled_axis));
}

void	ft_refracted_ray(t_cam *cam, t_light *light, t_vector normal)
{
	t_vector	scaled_normal;
	double		normal_scalar;

	cam->reversed_ray = ft_normalise_vector(ft_sub_vector(cam->position,
				cam->intersection));
	light->light_vect = ft_normalise_vector(ft_sub_vector(light->center,
				cam->intersection));
	normal_scalar = 2.0 * ft_dot_vector(normal, light->light_vect);
	scaled_normal = ft_scale_vector(normal, normal_scalar);
	light->reflected_light_vect = ft_normalise_vector(ft_sub_vector(
		scaled_normal, light->light_vect));
}
