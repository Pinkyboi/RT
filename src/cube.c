/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azarzor <azarzor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/01 05:27:09 by azarzor           #+#    #+#             */
/*   Updated: 2020/03/04 21:43:43 by azarzor          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void					ft_cube_normal(t_cam *cam, t_cube *cube)
{
	t_vector	center;
	t_vector	d;
	t_vector	hit;
	double		bias;

	bias = 1.00001;
	ft_intersection_position(cam, cam->hit.soluces[0]);
	center = ft_scale_vector(
		ft_add_vector(cube->bounds[0], cube->bounds[1]), 0.5);
	d = ft_scale_vector(
		ft_sub_vector(cube->bounds[0], cube->bounds[1]), 0.5);
	d.x = fabs(d.x) * bias;
	d.y = fabs(d.y) * bias;
	d.z = fabs(d.z) * bias;
	hit = ft_sub_vector(cam->hit.position, center);
	if (cam->hit.soluces[0])
	{
		cam->hit.normal = ft_normalise_vector((t_vector){hit.x / d.x,
				hit.y / d.y, hit.z / d.z});
		cam->hit.color = cube->color;
		cam->hit.reflection = cube->material.reflection_index;
		cam->hit.refraction = cube->material.refraction_index;
		cam->hit.transparency = cube->material.transparency_index;
	}
}

static double			ft_cube_calc(t_cam *cam, t_vector *sol, t_cube *cube)
{
	cam->hit.soluces[0] = sol[0].x;
	ft_cube_normal(cam, cube);
	return (cam->hit.soluces[0]);
}

double					ft_cube_intersection(t_cam *cam,
		t_cube *cube, double min)
{
	t_vector		sol[3];
	int				sign[3];

	sol[2] = (t_vector){1.0 / RAY_DIR.x, 1.0 / RAY_DIR.y, 1.0 / RAY_DIR.z};
	sign[0] = sol[2].x < MIN_D;
	sign[1] = sol[2].y < MIN_D;
	sign[2] = sol[2].z < MIN_D;
	sol[0].x = (cube->bounds[sign[0]].x - cam->ray_origin.x) * sol[2].x;
	sol[1].x = (cube->bounds[1 - sign[0]].x - cam->ray_origin.x) * sol[2].x;
	sol[0].y = (cube->bounds[sign[1]].y - cam->ray_origin.y) * sol[2].y;
	sol[1].y = (cube->bounds[1 - sign[1]].y - cam->ray_origin.y) * sol[2].y;
	if ((sol[0].x > sol[1].y) || (sol[0].y > sol[1].x))
		return (0);
	sol[0].x = (sol[0].y > sol[0].x) ? sol[0].y : sol[0].x;
	sol[1].x = (sol[1].y < sol[1].x) ? sol[1].y : sol[1].x;
	sol[0].z = (cube->bounds[sign[2]].z - cam->ray_origin.z) * sol[2].z;
	sol[1].z = (cube->bounds[1 - sign[2]].z - cam->ray_origin.z) * sol[2].z;
	if ((sol[0].x > sol[1].z) || (sol[0].z > sol[1].x))
		return (0);
	sol[0].x = (sol[0].z > sol[0].x) ? sol[0].z : sol[0].x;
	sol[1].x = (sol[1].z < sol[1].x) ? sol[1].z : sol[1].x;
	if (ft_check_min_distance(&sol[0].x, sol[1].x, min))
		return (ft_cube_calc(cam, sol, cube));
	return (0);
}
