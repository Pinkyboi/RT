/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection_shapes.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/19 18:58:17 by abenaiss          #+#    #+#             */
/*   Updated: 2020/01/05 10:47:27 by abenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include <stdio.h>

int				ft_check_min_distance(double *x1, double x2, double min)
{
	if ((x2 <= *x1 || *x1 < MIN_D) && x2 > MIN_D
			&& (x2 < min))
	{
		*x1 = x2;
		return (1);
	}
	else if ((*x1 < x2 || x2 < MIN_D) && *x1 > MIN_D
			&& (*x1 < min))
		return (1);
	else
		return (0);
}

double			ft_cylinder_intersection(t_cam *cam,
		t_cylinder *cylinder, double *min)
{
	double		abc[3];
	double		delta;
	t_vector	dist;

	dist = ft_sub_vector(cam->position, cylinder->center);
	A = 1 - FT_SQR(ft_dot_vector(cam->ray_direction, cylinder->axis));
	B = 2 * (ft_dot_vector(cam->ray_direction, dist)
			- (ft_dot_vector(cam->ray_direction, cylinder->axis) *
				ft_dot_vector(dist, cylinder->axis)));
	C = ft_dot_vector(dist, dist)
		- (ft_dot_vector(dist, cylinder->axis) *
				ft_dot_vector(dist, cylinder->axis))
		- (cylinder->radius * cylinder->radius);
	delta = (B * B) - (4 * A * C);
	if (delta < 0)
		return (0);
	cylinder->soluce[0] = (-B + sqrt(delta)) / (2 * A);
	cylinder->soluce[1] = (-B - sqrt(delta)) / (2 * A);
	if (ft_check_min_distance(&cylinder->soluce[0], cylinder->soluce[1], *min))
		ft_cylinder_normal(cam, cylinder, cylinder->soluce[0]);
	else
		cylinder->soluce[0] = 0;
	return (cylinder->soluce[0]);
}

double			ft_hyperboloid_intersection(t_cam *cam,
		t_hyperboloid *hyperboloid, double *min)
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
		hyperboloid->soluce[1], *min))
		ft_hyperboloid_normal(cam, hyperboloid, hyperboloid->soluce[0]);
	else
		hyperboloid->soluce[0] = 0;
	return (hyperboloid->soluce[0]);
}

double			ft_cone_intersection(t_cam *cam, t_cone *cone, double *min)
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
	if (ft_check_min_distance(&cone->soluce[0], cone->soluce[1], *min))
		ft_cone_normal(cam, cone, cone->soluce[0]);
	else
		cone->soluce[0] = 0;
	return (cone->soluce[0]);
}

double			ft_sphere_intersection(t_cam *cam,
		t_sphere *sphere, double *min)
{
	double	abc[3];
	double	delta;

	A = 1;
	B = 2 * ft_dot_vector(cam->ray_direction,
			ft_sub_vector(cam->position, sphere->center));
	C = FT_SQR(ft_vector_size(ft_sub_vector(cam->position, sphere->center)))
		- (sphere->radius * sphere->radius);
	delta = (B * B) - (4 * A * C);
	if (delta < 0)
		return (0);
	sphere->soluce[0] = (-B + sqrt(delta)) / 2;
	sphere->soluce[1] = (-B - sqrt(delta)) / 2;
	if (ft_check_min_distance(&sphere->soluce[0], sphere->soluce[1], *min))
		ft_sphere_normal(cam, sphere, sphere->soluce[0]);
	else
		sphere->soluce[0] = 0;
	return (sphere->soluce[0]);
}

double			ft_ellipsoid_intersection(t_cam *cam,
		t_ellipsoid *ellipsoid, double *min)
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
		ellipsoid->soluce[1], *min))
		ft_ellipsoid_normal(cam, ellipsoid, ellipsoid->soluce[0]);
	else
		ellipsoid->soluce[0] = 0;
	return (ellipsoid->soluce[0]);
}

double			ft_plane_intersection(t_cam *cam, t_plane *plane, double *min)
{
	double		i;
	t_vector	temp;

	i = ft_dot_vector(cam->ray_direction, plane->normal);
	if (fabs(i) > MIN_D)
	{
		temp = ft_sub_vector(plane->center, cam->position);
		plane->soluce[0] = ft_dot_vector(temp, plane->normal) / i;
		plane->soluce[1] = plane->soluce[0];
		if (plane->soluce[0] < *min && plane->soluce[0] > MIN_D)
		{
			if (i > 0)
				plane->normal = ft_scale_vector(plane->normal, -1);
			ft_intersection_position(cam, plane->soluce[0]);
			if (plane->radius < 0 || plane->radius >=
				ft_vector_size(ft_sub_vector(plane->center, cam->intersection)))
				return (plane->soluce[0]);
		}
	}
	return (0);
}

double			ft_triangle_intersection(t_cam *cam,
	t_triangle *triangle, double *min)
{
	double		det;
	double		test[2];
	t_vector	temp[3];

	temp[0] = ft_cross_product(cam->ray_direction, triangle->side[1]);
	det = ft_dot_vector(triangle->side[0], temp[0]);
	if (det > MIN_D)
	{
		temp[1] = ft_sub_vector(cam->position, triangle->center);
		test[0] = ft_dot_vector(temp[1], temp[0]) / det;
		if (test[0] < MIN_D || test[0] > 1)
			return (0);
		temp[2] = ft_cross_product(temp[1], triangle->side[0]);
		test[1] = ft_dot_vector(cam->ray_direction, temp[2]) / det;
		if (test[1] < MIN_D || test[0] + test[1] > 1.0)
			return (0);
		triangle->soluce[0] = ft_dot_vector(triangle->side[1], temp[2]) / det;
		if ((triangle->soluce[0]) < *min && triangle->soluce[0] > MIN_D)
		{
			ft_intersection_position(cam, triangle->soluce[0]);
			return (triangle->soluce[0]);
		}
	}
	return (0);
}
