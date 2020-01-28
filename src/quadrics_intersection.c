/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quadrics_intersection.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azarzor <azarzor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/17 17:18:42 by abiri             #+#    #+#             */
/*   Updated: 2020/01/20 18:56:59 by azarzor          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

double			ft_hyperboloid_intersection(t_cam *cam,
		t_hyperboloid *hyperboloid, double min)
{
	double		abc[3];
	double		delta;
	t_vector	dist;

	dist = ft_sub_vector(cam->ray_origin, hyperboloid->center);
	A = FT_SQR(cam->ray_direction.x) +
		FT_SQR(cam->ray_direction.z) - FT_SQR(cam->ray_direction.y);
	B = 2.0 * ((dist.x * cam->ray_direction.x) +
		(dist.z * cam->ray_direction.z) - (dist.y * cam->ray_direction.y));
	C = FT_SQR(dist.x) + FT_SQR(dist.z) - (FT_SQR(dist.y) +
		((hyperboloid->sheets == 1) ?
		hyperboloid->coefficient : -hyperboloid->coefficient));
	delta = (B * B) - (4 * A * C);
	if (delta < 0)
		return (0);
	cam->hit.soluces[0] = (-B + sqrt(delta)) / (2 * A);
	cam->hit.soluces[1] = (-B - sqrt(delta)) / (2 * A);
	if (ft_check_min_distance(&cam->hit.soluces[0],
		cam->hit.soluces[1], min))
		ft_hyperboloid_normal(cam, hyperboloid, cam->hit.soluces[0]);
	else
		cam->hit.soluces[0] = 0;
	return (cam->hit.soluces[0]);
}

double			ft_paraboloid_intersection(t_cam *cam,
		t_paraboloid *paraboloid, double min)
{
	double		abc[3];
	double		delta;
	t_vector	dist;

	dist = ft_sub_vector(cam->ray_origin, paraboloid->center);
	A = FT_SQR(cam->ray_direction.x) +
		FT_SQR(cam->ray_direction.z);
	B = 2.0 * ((dist.x * cam->ray_direction.x) +
		(dist.z * cam->ray_direction.z)) - cam->ray_direction.y;
	C = FT_SQR(dist.x) + FT_SQR(dist.z) - (dist.y + paraboloid->coefficient);
	delta = (B * B) - (4 * A * C);
	if (delta < 0)
		return (0);
	cam->hit.soluces[0] = (-B + sqrt(delta)) / (2 * A);
	cam->hit.soluces[1] = (-B - sqrt(delta)) / (2 * A);
	if (ft_check_min_distance(&cam->hit.soluces[0],
		cam->hit.soluces[1], min))
		ft_paraboloid_normal(cam, paraboloid, cam->hit.soluces[0]);
	else
		cam->hit.soluces[0] = 0;
	return (cam->hit.soluces[0]);
}

double			ft_ellipsoid_intersection(t_cam *cam,
		t_ellipsoid *ellipsoid, double min)
{
	double	abc[3];
	double	delta;

	A = FT_SQR(ft_vector_size(
		ft_div_vector(cam->ray_direction, ellipsoid->axis)));
	B = 2 * ft_dot_vector(
			ft_div_vector(cam->ray_direction, ellipsoid->axis),
			ft_sub_vector(ft_div_vector(cam->ray_origin, ellipsoid->axis),
			ft_div_vector(ellipsoid->center, ellipsoid->axis)));
	C = FT_SQR(ft_vector_size(ft_sub_vector(
		ft_div_vector(cam->ray_origin, ellipsoid->axis),
		ft_div_vector(ellipsoid->center, ellipsoid->axis)))) - 1;
	delta = (B * B) - (4 * A * C);
	if (delta < 0)
		return (0);
	cam->hit.soluces[0] = (-B + sqrt(delta)) / (2 * A);
	cam->hit.soluces[1] = (-B - sqrt(delta)) / (2 * A);
	if (ft_check_min_distance(&cam->hit.soluces[0],
		cam->hit.soluces[1], min))
		ft_ellipsoid_normal(cam, ellipsoid, cam->hit.soluces[0]);
	else
		cam->hit.soluces[0] = 0;
	return (cam->hit.soluces[0]);
}

double			ft_cone_intersection(t_cam *cam, t_cone *cone, double min)
{
	double		abc[3];
	double		delta;
	t_vector	dist;

	dist = ft_sub_vector(cam->ray_origin, cone->center);
	A = 1 - ((1 + FT_SQR(cone->tilt)) * FT_SQR(ft_dot_vector(cam->ray_direction,
					cone->axis)));
	B = 2 * (ft_dot_vector(cam->ray_direction, dist) -
			((1 + FT_SQR(cone->tilt)) *
			(ft_dot_vector(cam->ray_direction, cone->axis) *
			ft_dot_vector(dist, cone->axis))));
	C = ft_dot_vector(dist, dist) - ((1 + FT_SQR(cone->tilt))) *
		(FT_SQR(ft_dot_vector(dist, cone->axis)));
	delta = (B * B) - (4 * A * C);
	if (delta < 0)
		return (0);
	cam->hit.soluces[0] = (-B + sqrt(delta)) / (2 * A);
	cam->hit.soluces[1] = (-B - sqrt(delta)) / (2 * A);
	if (ft_check_min_distance(&cam->hit.soluces[0], cam->hit.soluces[1], min))
		ft_cone_normal(cam, cone, cam->hit.soluces[0]);
	else
		cam->hit.soluces[0] = 0;
	return (cam->hit.soluces[0]);
}
