/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   torus.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azarzor <azarzor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/01 00:33:33 by azarzor           #+#    #+#             */
/*   Updated: 2020/03/04 21:25:14 by azarzor          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void					ft_torus_normal(t_cam *cam, t_torus *torus)
{
	t_vector	distance;
	t_vector	normal;

	ft_intersection_position(cam, cam->hit.soluces[0]);
	normal = ft_new_vector(0, 0, 0);
	distance = ft_add_vector(cam->ray_origin,
			ft_scale_vector(cam->ray_direction, cam->hit.soluces[0]));
	normal.x = 4.0 * distance.x * (FT_SQR(distance.x) + FT_SQR(distance.y) +
			FT_SQR(distance.z) - FT_SQR(torus->s_radius)
			- FT_SQR(torus->b_radius));
	normal.y = 4.0 * distance.y * (FT_SQR(distance.x) + FT_SQR(distance.y) +
			FT_SQR(distance.z) - FT_SQR(torus->s_radius) -
			FT_SQR(torus->b_radius) + 2.0 * FT_SQR(torus->b_radius));
	normal.z = 4.0 * distance.z * (FT_SQR(distance.x) + FT_SQR(distance.y) +
			FT_SQR(distance.z) - FT_SQR(torus->s_radius) -
			FT_SQR(torus->b_radius));
	if (cam->hit.soluces[0])
	{
		cam->hit.normal = ft_normalise_vector(normal);
		cam->hit.color = torus->color;
		cam->hit.reflection = torus->material.reflection_index;
		cam->hit.refraction = torus->material.refraction_index;
		cam->hit.transparency = torus->material.transparency_index;
	}
}

static double			ft_torus_calc(t_cam *cam, t_torus *torus, double min)
{
	if (cam->hit.soluces[0] > MIN_D && cam->hit.soluces[0] < min)
	{
		ft_torus_normal(cam, torus);
		return (cam->hit.soluces[0]);
	}
	else
		cam->hit.soluces[0] = 0;
	return (cam->hit.soluces[0]);
}

double					ft_torus_intersection(t_cam *cam,
			t_torus *torus, double min)
{
	int			result;
	t_vector	center;
	double		coeff[5];
	double		sol[4];

	center = ft_sub_vector(cam->ray_origin, torus->center);
	C4 = FT_SQR(ft_dot_vector(RAY_DIR, RAY_DIR));
	C3 = 4.0 * (FT_SQR(RAY_DIR.x) + FT_SQR(RAY_DIR.y) + FT_SQR(RAY_DIR.z))
		* (center.x * RAY_DIR.x + center.y * RAY_DIR.y + center.z * RAY_DIR.z);
	C2 = 2.0 * (FT_SQR(RAY_DIR.x) + FT_SQR(RAY_DIR.y) + FT_SQR(RAY_DIR.z))
		* (FT_SQR(center.x) + FT_SQR(center.y) + FT_SQR(center.z) -
		(FT_SQR(torus->s_radius) + FT_SQR(torus->b_radius))) +
		4.0 * FT_SQR((center.x * RAY_DIR.x + center.y * RAY_DIR.y
	+ center.z * RAY_DIR.z)) + 4.0 * FT_SQR(torus->b_radius) *
	RAY_DIR.y * RAY_DIR.y;
	C1 = 4.0 * ((FT_SQR(center.x) + FT_SQR(center.y) + FT_SQR(center.z))
		- (FT_SQR(torus->s_radius) + FT_SQR(torus->b_radius)))
		* (center.x * RAY_DIR.x + center.y * RAY_DIR.y + center.z *
		RAY_DIR.z) + 8.0 * FT_SQR(torus->b_radius) * center.y * RAY_DIR.y;
	C0 = FT_SQR((FT_SQR(center.x) + FT_SQR(center.y) + FT_SQR(center.z)
		- (FT_SQR(torus->s_radius) + FT_SQR(torus->b_radius)))) - 4.0
	* (FT_SQR(torus->b_radius)) * (FT_SQR(torus->s_radius) - FT_SQR(center.y));
	result = ft_solve_quartic(coeff, sol);
	cam->hit.soluces[0] = ft_min_sol(sol, min, result);
	return (ft_torus_calc(cam, torus, min));
}
