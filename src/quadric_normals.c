/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quadric_normals.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiri <abiri@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/07 23:15:12 by abenaiss          #+#    #+#             */
/*   Updated: 2020/01/13 10:38:47 by abiri            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void	ft_ellipsoid_normal(t_cam *cam, t_ellipsoid *ellipsoid, double distance)
{
	t_vector	normal;
	t_vector	radius;

	ft_intersection_position(cam, distance);
	radius = ft_sub_vector(cam->hit.position, ellipsoid->center);
	normal.x = 2 * radius.x / FT_SQR(ellipsoid->axis.x);
	normal.y = 2 * radius.y / FT_SQR(ellipsoid->axis.y);
	normal.z = 2 * radius.z / FT_SQR(ellipsoid->axis.z);
	if (cam->hit.soluces[0])
	{
		cam->hit.normal = ft_normalise_vector(normal);
		cam->hit.color = ellipsoid->color;
		cam->hit.reflection = ellipsoid->reflection;
		cam->hit.refraction = ellipsoid->refraction;
	}
}

void	ft_hyperboloid_normal(t_cam *cam, t_hyperboloid *hyperboloid,
	double distance)
{
	t_vector	normal;
	t_vector	radius;

	ft_intersection_position(cam, distance);
	radius = ft_sub_vector(cam->hit.position, hyperboloid->center);
	normal.x = 2 * radius.x;
	normal.y = -2 * radius.y;
	normal.z = 2 * radius.z;
	if (cam->hit.soluces[0])
	{
		cam->hit.normal = ft_normalise_vector(normal);
		cam->hit.color = hyperboloid->color;
		cam->hit.reflection = hyperboloid->reflection;
		cam->hit.refraction = hyperboloid->refraction;
	}
}

void	ft_paraboloid_normal(t_cam *cam, t_paraboloid *paraboloid,
	double distance)
{
	t_vector	normal;
	t_vector	radius;

	ft_intersection_position(cam, distance);
	radius = ft_sub_vector(cam->hit.position, paraboloid->center);
	normal.x = 2 * radius.x;
	normal.y = -1;
	normal.z = 2 * radius.z;
	if (cam->hit.soluces[0])
	{
		cam->hit.normal = ft_normalise_vector(normal);
		cam->hit.color = paraboloid->color;
		cam->hit.reflection = paraboloid->reflection;
		cam->hit.refraction = paraboloid->refraction;
	}
}