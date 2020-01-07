/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quadrics_intersection.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/17 17:18:42 by abiri             #+#    #+#             */
/*   Updated: 2020/01/06 11:51:45 by abenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

double			ft_hyperboloid_intersection(t_cam *cam,
		t_hyperboloid *hyperboloid, double min)
{
	double		abc[3];
	double		delta;
	t_vector	dist;

	dist = ft_sub_vector(cam->position, hyperboloid->center);
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
	hyperboloid->soluce[0] = (-B + sqrt(delta)) / (2 * A);
	hyperboloid->soluce[1] = (-B - sqrt(delta)) / (2 * A);
	if (ft_check_min_distance(&hyperboloid->soluce[0],
		hyperboloid->soluce[1], min))
		ft_hyperboloid_normal(cam, hyperboloid, hyperboloid->soluce[0]);
	else
		hyperboloid->soluce[0] = 0;
	return (hyperboloid->soluce[0]);
}

double			ft_paraboloid_intersection(t_cam *cam,
		t_paraboloid *paraboloid, double min)
{
	double		abc[3];
	double		delta;
	t_vector	dist;

	dist = ft_sub_vector(cam->position, paraboloid->center);
	A = FT_SQR(cam->ray_direction.x) +
		FT_SQR(cam->ray_direction.z);
	B = 2.0 * ((dist.x * cam->ray_direction.x) +
		(dist.z * cam->ray_direction.z)) - cam->ray_direction.y;
	C = FT_SQR(dist.x) + FT_SQR(dist.z) - (dist.y + paraboloid->coefficient);
	delta = (B * B) - (4 * A * C);
	if (delta < 0)
		return (0);
	paraboloid->soluce[0] = (-B + sqrt(delta)) / (2 * A);
	paraboloid->soluce[1] = (-B - sqrt(delta)) / (2 * A);
	if (ft_check_min_distance(&paraboloid->soluce[0],
		paraboloid->soluce[1], min))
		ft_paraboloid_normal(cam, paraboloid, paraboloid->soluce[0]);
	else
		paraboloid->soluce[0] = 0;
	return (paraboloid->soluce[0]);
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
			ft_sub_vector(ft_div_vector(cam->position, ellipsoid->axis),
			ft_div_vector(ellipsoid->center, ellipsoid->axis)));
	C = FT_SQR(ft_vector_size(ft_sub_vector(
		ft_div_vector(cam->position, ellipsoid->axis),
		ft_div_vector(ellipsoid->center, ellipsoid->axis)))) - 1;
	delta = (B * B) - (4 * A * C);
	if (delta < 0)
		return (0);
	ellipsoid->soluce[0] = (-B + sqrt(delta)) / (2 * A);
	ellipsoid->soluce[1] = (-B - sqrt(delta)) / (2 * A);
	if (ft_check_min_distance(&ellipsoid->soluce[0],
		ellipsoid->soluce[1], min))
		ft_ellipsoid_normal(cam, ellipsoid, ellipsoid->soluce[0]);
	else
		ellipsoid->soluce[0] = 0;
	return (ellipsoid->soluce[0]);
}

double			ft_cone_intersection(t_cam *cam, t_cone *cone, double min)
{
	double		abc[3];
	double		delta;
	t_vector	dist;

	dist = ft_sub_vector(cam->position, cone->center);
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
	cone->soluce[0] = (-B + sqrt(delta)) / (2 * A);
	cone->soluce[1] = (-B - sqrt(delta)) / (2 * A);
	if (ft_check_min_distance(&cone->soluce[0], cone->soluce[1], min))
		ft_cone_normal(cam, cone, cone->soluce[0]);
	else
		cone->soluce[0] = 0;
	return (cone->soluce[0]);
}
