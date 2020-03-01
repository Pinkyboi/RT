/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   holo_cube.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azarzor <azarzor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 07:00:38 by abenaiss          #+#    #+#             */
/*   Updated: 2020/03/01 17:15:12 by azarzor          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

double					ft_min_sol(double s[4], int res)
{
	double	min;
	int		i;

	if (res == 0)
		return (0);
	min = MIN_D;
	i = -1;
	while (++i < res)
		if (s[i] > MIN_D)
		{
			min = s[i];
			break ;
		}
	if (i == res)
		return (0);
	i = -1;
	while (++i < res)
	{
		if (min > MIN_D && min < s[i])
			min = s[i];
	}
	if (min > MIN_D)
		return (min);
	return (0.0);
}

void					ft_holo_cube_normal(t_cam *cam, t_holo_cube *holo_cube)
{
	t_vector	distance;
	t_vector	normal;

	ft_intersection_position(cam, cam->hit.soluces[0]);
	distance = ft_add_vector(cam->ray_origin,
			ft_scale_vector(cam->ray_direction, cam->hit.soluces[0]));
	normal.x = 4 * FT_CUB(distance.x) - 10 * distance.x;
	normal.y = 4 * FT_CUB(distance.y) - 10 * distance.y;
	normal.z = 4 * FT_CUB(distance.z) - 10 * distance.z;
	holo_cube->normal = ft_normalise_vector(normal);
	if (cam->hit.soluces[0])
	{
		cam->hit.normal = ft_normalise_vector(normal);
		cam->hit.color = holo_cube->color;
		cam->hit.reflection = holo_cube->material.reflection_index;
		cam->hit.refraction = holo_cube->material.refraction_index;
	}
}

static	double			ft_holo_cube_calc(t_cam *cam,
			t_holo_cube *holo_cube, double min)
{
	if (cam->hit.soluces[0] > MIN_D && cam->hit.soluces[0] < min)
	{
		ft_holo_cube_normal(cam, holo_cube);
		return (cam->hit.soluces[0]);
	}
	return (0);
}

double					ft_holo_cube_intersection(t_cam *cam,
			t_holo_cube *holo_cube, double min)
{
	int			result;
	t_vector	center;
	double		coeff[5];
	double		sol[4];

	center = ft_sub_vector(cam->ray_origin, holo_cube->center);
	C4 = FT_QUAR(cam->ray_direction.x) +
			FT_QUAR(cam->ray_direction.y) + FT_QUAR(cam->ray_direction.z);
	C3 = 4.0 * (FT_CUB(cam->ray_direction.x) * center.x
				+ FT_CUB(cam->ray_direction.y) * center.y
				+ FT_CUB(cam->ray_direction.z) * center.z);
	C2 = 6.0 * (FT_SQR(cam->ray_direction.x) * FT_SQR(center.x)
				+ FT_SQR(cam->ray_direction.y) * FT_SQR(center.y)
				+ FT_SQR(cam->ray_direction.z) * FT_SQR(center.z))
			- 5.0 * ft_dot_vector(cam->ray_direction, cam->ray_direction);
	C1 = 4.0 * (FT_CUB(center.x) * cam->ray_direction.x
				+ FT_CUB(center.y) * cam->ray_direction.y
				+ FT_CUB(center.z) * cam->ray_direction.z) - 10.0
				* ft_dot_vector(cam->ray_direction, center);
	C0 = FT_QUAR(center.x) + FT_QUAR(center.y) + FT_QUAR(center.z)
			- 5.0 * ft_dot_vector(center, center) + holo_cube->radius;
	result = ft_solve_quartic(coeff, sol);
	cam->hit.soluces[0] = ft_min_sol(sol, result);
	return (ft_holo_cube_calc(cam, holo_cube, min));
}
