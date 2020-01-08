/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   basics_intersection.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/19 18:58:17 by abenaiss          #+#    #+#             */
/*   Updated: 2020/01/08 14:16:38 by abenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

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
		t_cylinder *cylinder, double min)
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
	cam->hit.soluces[0] = (-B + sqrt(delta)) / (2 * A);
	cam->hit.soluces[1] = (-B - sqrt(delta)) / (2 * A);
	if (ft_check_min_distance(&cam->hit.soluces[0], cam->hit.soluces[1], min))
		ft_cylinder_normal(cam, cylinder, cam->hit.soluces[0]);
	else
		cam->hit.soluces[0] = 0;
	return (cam->hit.soluces[0]);
}

double			ft_sphere_intersection(t_cam *cam,
		t_sphere *sphere, double min)
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
	cam->hit.soluces[0] = (-B + sqrt(delta)) / 2;
	cam->hit.soluces[1] = (-B - sqrt(delta)) / 2;
	if (ft_check_min_distance(&cam->hit.soluces[0], cam->hit.soluces[1], min))
		ft_sphere_normal(cam, sphere, cam->hit.soluces[0]);
	else
		cam->hit.soluces[0] = 0;
	return (cam->hit.soluces[0]);
}

t_color			ft_cheeker_texture(double x, double y, double scale)
{
	if ((x * scale) - floor(x * scale) < 0.5 ||
		(y * scale) - floor(y * scale) < 0.5)
	{
		if ((x * scale) - floor(x * scale) < 0.5 &&
			(y * scale) - floor(y * scale) < 0.5)
			return ((t_color){0, 0, 0});
		else
			return ((t_color){1, 1, 1});
	}
	else
		return ((t_color){0, 0, 0});
}

t_color				ft_map_texture(t_cam *cam, t_plane plane)
{
	t_vector	up;
	t_vector	sides[2];
	double		x[2];

	up = ft_cross_product(ft_rotate_vector(plane.normal,
		(t_vector){90, 90, 90}), plane.normal);
	sides[0] = ft_normalise_vector(
			ft_cross_product(up, plane.normal));
	sides[1] = ft_normalise_vector(
			ft_cross_product(sides[0], plane.normal));
	x[0] = ft_dot_vector(
			ft_sub_vector(cam->hit.position, plane.center), sides[0]);
	x[1] = ft_dot_vector(
			ft_sub_vector(cam->hit.position, plane.center), sides[1]);
	return(ft_cheeker_texture(x[1], x[0], 5));
}	

double			ft_plane_intersection(t_cam *cam, t_plane *plane, double min)
{
	double		i;
	t_vector	temp;

	i = ft_dot_vector(cam->ray_direction, plane->normal);
	if (fabs(i) > MIN_D)
	{
		temp = ft_sub_vector(plane->center, cam->position);
		cam->hit.soluces[0] = ft_dot_vector(temp, plane->normal) / i;
		if (cam->hit.soluces[0] < min && cam->hit.soluces[0] > MIN_D)
		{
			ft_intersection_position(cam, cam->hit.soluces[0]);
			if (plane->radius < 0 || plane->radius >=
				ft_vector_size(ft_sub_vector(plane->center, cam->hit.position)))
			{
				cam->hit.normal = (i > 0) ?
					ft_scale_vector(plane->normal, -1) : plane->normal;
				cam->hit.color = plane->color;
				return (cam->hit.soluces[0]);
			}
		}
	}
	return (0);
}

double			ft_triangle_intersection(t_cam *cam,
	t_triangle *triangle, double min)
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
		cam->hit.soluces[0] = ft_dot_vector(triangle->side[1], temp[2]) / det;
		if (!(test[1] < MIN_D || test[0] + test[1] > 1.0)
			&& ((cam->hit.soluces[0]) < min && cam->hit.soluces[0] > MIN_D))
		{
			cam->hit.normal = triangle->normal;
			cam->hit.color = triangle->color;
			ft_intersection_position(cam, cam->hit.soluces[0]);
			return (cam->hit.soluces[0]);
		}
	}
	return (0);
}
