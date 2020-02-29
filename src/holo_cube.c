/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   holo_cube.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 07:00:38 by abenaiss          #+#    #+#             */
/*   Updated: 2020/02/29 07:00:40 by abenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void			ft_holo_cube_normal(t_cam *cam, t_holo_cube *holo_cube)
{
	t_vector	distance;
	t_vector	normal;

	ft_intersection_position(cam, cam->hit.soluces[0]);
	distance = ft_add_vector(cam->ray_origin,
			ft_scale_vector(cam->ray_direction, cam->hit.soluces[0]));
	normal.x = (4 * FT_SQR(distance.x) * distance.x) - (10 * distance.x);
	normal.y = (4 * FT_SQR(distance.y) * distance.y) - (10 * distance.y);
	normal.z = (4 * FT_SQR(distance.z) * distance.z) - (10 * distance.z);
	if (cam->hit.soluces[0])
	{
		cam->hit.normal = ft_normalise_vector(normal);
		cam->hit.color = holo_cube->color;
		cam->hit.reflection = holo_cube->material.reflection_index;
		cam->hit.refraction = holo_cube->material.refraction_index;
	}
}

double			ft_holo_cube_intersection(t_cam *cam,
	t_holo_cube *holo_cube, double min)
{
	t_vector	dist;
	double		soluces[5];

	dist = ft_sub_vector(cam->ray_origin, holo_cube->center);
	soluces[0] = (FT_SQR(cam->ray_direction.x) * FT_SQR(cam->ray_direction.x))
		+ (FT_SQR(cam->ray_direction.y) * FT_SQR(cam->ray_direction.y))
		+ (FT_SQR(cam->ray_direction.z) * FT_SQR(cam->ray_direction.z));
	soluces[1] = 4.0 *
		((FT_SQR(cam->ray_direction.x) * cam->ray_direction.x * dist.x)
		+ (FT_SQR(cam->ray_direction.y) * cam->ray_direction.y * dist.y)
		+ (FT_SQR(cam->ray_direction.z) * cam->ray_direction.z * dist.z));
	soluces[2] = (6.0 * (FT_SQR(cam->ray_direction.x) * FT_SQR(dist.x)
		+ FT_SQR(cam->ray_direction.y * FT_SQR(dist.y)
		+ FT_SQR(cam->ray_direction.z) * FT_SQR(dist.z))))
		- (5.0 * ft_dot_vector(cam->ray_direction, cam->ray_direction));
	soluces[3] = 4.0 * (((FT_SQR(dist.x) * dist.x) * cam->ray_direction.x)
		+ ((FT_SQR(dist.y) * dist.y) * cam->ray_direction.y)
		+ ((FT_SQR(dist.z) * dist.z) * cam->ray_direction.z))
		- (10.0 * ft_dot_vector(cam->ray_direction, dist));
	soluces[4] = (FT_SQR(dist.x) * FT_SQR(dist.x))
		+ (FT_SQR(dist.y) * FT_SQR(dist.y))
		+ ((FT_SQR(dist.y) * FT_SQR(dist.y)))
		- (5.0 * ft_dot_vector(dist, dist) + holo_cube->radius);
	return (0);
}
