/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cam_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/19 18:58:05 by abenaiss          #+#    #+#             */
/*   Updated: 2020/02/22 05:45:31 by abenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include <stdio.h>

t_vector	ft_rotate_vector(t_vector a, t_vector angles)
{
	t_vector	result;

	result = ft_rotate_vectorx(a, FT_RAD(angles.x));
	result = ft_rotate_vectory(result, FT_RAD(angles.y));
	result = ft_rotate_vectorz(result, FT_RAD(angles.z));
	return (ft_normalise_vector(result));
}

void		ft_intersection_position(t_cam *cam, double first_intersection)
{
	t_vector	distance;

	distance = ft_scale_vector(cam->ray_direction, first_intersection);
	cam->hit.position = ft_add_vector(cam->ray_origin, distance);
}

/**
 * FIX YOUR FUNCTIONS DOLAN
**/
void		ft_depth_of_field(t_rtv *rtv, int sample)
{
	t_vector			world_point;
	t_vector			rd;
	t_vector			offset;
	t_vector			point_aim;
	double				radius;
	static double		anti_aliazing[9][2] = {
		{0, 0},
		{-1.0 / 4.0, 3.0 / 4.0},
		{3.0 / 4.0, 1.0 / 4.0},
		{-3.0 / 4.0, -1.0 / 4.0},
		{ 1.0 / 4.0, -3.0 / 4.0},
		{-1.0 / 4.0, -1.0 / 4.0},
		{1.0 / 4.0, 1.0 / 4.0},
		{3.0 / 4.0, 3.0 / 4.0},
		{-3.0 / 4.0, -3.0 / 4.0},
	};
	
	rd.x = rand() / (float)(RAND_MAX + 1);
	rd.y = rand() / (float)(RAND_MAX + 1);
	rd.z = 0.0;
	radius = 0.3;
	offset = ft_add_vector(ft_scale_vector(rtv->cam.right, rd.x), ft_scale_vector(rtv->cam.up, rd.y));
	world_point = ft_add_vector(rtv->cam.bottom_left,
				ft_add_vector(ft_scale_vector(rtv->cam.h_scalar,
				(rtv->column + anti_aliazing[sample][0] + 0.5) / rtv->scene.height),
				ft_scale_vector(rtv->cam.w_scalar,
				(rtv->row + anti_aliazing[sample][1] + 0.5) / rtv->scene.width)));
	rtv->cam.ray_direction = ft_sub_vector(world_point, rtv->cam.position);
	point_aim = ft_add_vector(rtv->cam.position, ft_scale_vector(rtv->cam.ray_direction, 20));
	rtv->cam.ray_direction = ft_normalise_vector(rtv->cam.ray_direction);
	rtv->cam.ray_origin = ft_sub_vector(rtv->cam.position,
		ft_add_vector(ft_sub_vector(ft_scale_vector(rtv->cam.right, (double)radius / 2),
		ft_scale_vector(rtv->cam.up, (double)radius / 2)),
		ft_add_vector(ft_scale_vector(rtv->cam.right, (double)radius * rd.x),
		ft_scale_vector(rtv->cam.up, (double)radius * rd.y))));
	rtv->cam.ray_direction = ft_normalise_vector(ft_sub_vector(point_aim, rtv->cam.ray_origin));
}

void		ft_create_ray(t_rtv *rtv, int sample)
{
	t_vector			world_point;
	static double		anti_aliazing[9][2] = {
		{0, 0},
		{-1.0 / 4.0, 3.0 / 4.0},
		{3.0 / 4.0, 1.0 / 4.0},
		{-3.0 / 4.0, -1.0 / 4.0},
		{ 1.0 / 4.0, -3.0 / 4.0},
		{-1.0 / 4.0, -1.0 / 4.0},
		{1.0 / 4.0, 1.0 / 4.0},
		{3.0 / 4.0, 3.0 / 4.0},
		{-3.0 / 4.0, -3.0 / 4.0},
	};

	world_point = ft_add_vector(rtv->cam.bottom_left,
	ft_add_vector(ft_scale_vector(rtv->cam.h_scalar,
	(rtv->column + anti_aliazing[sample][0] + 0.5) / rtv->scene.height),
	ft_scale_vector(rtv->cam.w_scalar,
	(rtv->row + anti_aliazing[sample][1] + 0.5) / rtv->scene.width)));
	world_point = ft_sub_vector(world_point, rtv->cam.position);
	rtv->cam.ray_direction = ft_normalise_vector(world_point);
	rtv->cam.ray_origin = rtv->cam.position;
	//ft_depth_of_field(rtv, sample);
}

void		ft_init_cam(t_cam *cam, t_rtv rtv)
{
	t_vector	n_up;
	t_vector	cam_vects[4];
	double		cam_utils[3];

	n_up = ft_new_vector(MIN_D, 1 + MIN_D, MIN_D);
	cam->position = ft_add_vector(cam->position, cam->translation);
	CAM_FOREWORD = ft_normalise_vector(ft_sub_vector(cam->look_at,
			cam->position));
	CAM_RIGHT = ft_normalise_vector(
					ft_cross_product(CAM_FOREWORD, n_up));
	CAM_UP = ft_normalise_vector(ft_cross_product(CAM_RIGHT,
					ft_scale_vector(CAM_FOREWORD, -1)));
	RATIO = (double)rtv.scene.height / (double)rtv.scene.width;
	HALF_HEIGHT = tan(FT_RAD(cam->fov) / 2);
	HALF_WIDTH = HALF_HEIGHT / RATIO;
	cam->bottom_left = ft_sub_vector(cam->position, ft_add_vector(
					ft_scale_vector(CAM_UP, HALF_HEIGHT),
					ft_scale_vector(CAM_RIGHT, HALF_WIDTH)));
	cam->bottom_left = ft_add_vector(cam->bottom_left, CAM_FOREWORD);
	cam->w_scalar = ft_scale_vector(CAM_RIGHT, 2.0 * HALF_WIDTH);
	cam->h_scalar = ft_scale_vector(CAM_UP, 2.0 * HALF_HEIGHT);
	cam->right = CAM_RIGHT;
}