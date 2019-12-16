/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection_shapes.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/19 18:58:17 by abenaiss          #+#    #+#             */
/*   Updated: 2019/12/16 06:11:03 by abenaiss         ###   ########.fr       */
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
		t_cylinder *cylinder, double *min)
{
	double		abc[3];
	double		x[2];
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
	x[0] = (-B + sqrt(delta)) / (2 * A);
	x[1] = (-B - sqrt(delta)) / (2 * A);
	if (ft_check_min_distance(&x[0], x[1], *min))
		ft_cylinder_normal(cam, cylinder, x[0]);
	else
		x[0] = 0;
	return ((cylinder->limited && (fabs(cylinder->lenght) >=
		cylinder->max_lenght / 2)) ? 0 : x[0]);
}

double			ft_cone_intersection(t_cam *cam, t_cone *cone, double *min)
{
	double		abc[3];
	double		x[2];
	double		delta;
	t_vector	dist;

	dist = ft_sub_vector(cam->position, cone->center);
	A = 1 - ((1 + cone->tilt) * FT_SQR(ft_dot_vector(cam->ray_direction,
					cone->axis)));
	B = 2 * (ft_dot_vector(cam->ray_direction, dist) - ((1 + cone->tilt) *
			(ft_dot_vector(cam->ray_direction, cone->axis) *
			ft_dot_vector(dist, cone->axis))));
	C = ft_dot_vector(dist, dist) - ((1 + cone->tilt)) *
		(FT_SQR(ft_dot_vector(dist, cone->axis)));
	delta = (B * B) - (4 * A * C);
	if (delta < 0)
		return (0);
	x[0] = (-B + sqrt(delta)) / (2 * A);
	x[1] = (-B - sqrt(delta)) / (2 * A);
	if (ft_check_min_distance(&x[0], x[1], *min))
		ft_cone_normal(cam, cone, x[0]);
	else
		x[0] = 0;
	return ((cone->limited && ((cone->lenght >=
		cone->max_lenght) || !cone->lenght)) ? 0 : x[0]);
}

double			ft_sphere_intersection(t_cam *cam,
		t_sphere *sphere, double *min)
{
	double	abc[3];
	double	x[2];
	double	delta;

	A = 1;
	B = 2 * ft_dot_vector(cam->ray_direction,
			ft_sub_vector(cam->position, sphere->center));
	C = FT_SQR(ft_vector_size(ft_sub_vector(cam->position, sphere->center)))
		- (sphere->radius * sphere->radius);
	delta = (B * B) - (4 * A * C);
	if (delta < 0)
		return (0);
	x[0] = (-B + sqrt(delta)) / 2;
	x[1] = (-B - sqrt(delta)) / 2;
	if (ft_check_min_distance(&x[0], x[1], *min))
		ft_sphere_normal(cam, sphere, x[0]);
	else
		x[0] = 0;
	return (x[0]);
}

double			ft_plane_intersection(t_cam *cam, t_plane *plane, double *min)
{
	double		k;
	double		i;
	t_vector	temp;

	i = ft_dot_vector(cam->ray_direction, plane->normal);
	if (fabs(i) > MIN_D)
	{
		temp = ft_sub_vector(plane->center, cam->position);
		k = ft_dot_vector(temp, plane->normal) / i;
		if (k < *min && k > MIN_D)
		{
			if (i > 0)
				plane->normal = ft_scale_vector(plane->normal, -1);
			ft_intersection_position(cam, k);
			return (k);
		}
	}
	return (0);
}
