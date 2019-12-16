/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cam_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/19 18:58:05 by abenaiss          #+#    #+#             */
/*   Updated: 2019/12/16 00:26:30 by abenaiss         ###   ########.fr       */
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
	cam->intersection = ft_add_vector(cam->position, distance);
}

void		ft_create_ray(t_rtv *rtv, int sample)
{
	t_vector			world_point;
	static double		anti_aliazing[9][2] = {
		{0, 0},
		{-1.0 / 4.0, 3.0 / 4.0},
		{3.0 / 4.0, 1.0 / 4.0},
		{-3.0 / 4.0, -1.0 / 4.0},
		{ 1.0 / 6.0, -3.0 / 4.0},
		{-1.0 / 4.0, -1.0 / 4.0},
		{1.0 / 4.0, 1.0 / 4.0},
		{3.0 / 4.0, 3.0 / 4.0},
		{-3.0 / 4.0, -3.0 / 4.0},
	};

	world_point = ft_add_vector(rtv->cam.bottom_left,
				ft_add_vector(ft_scale_vector(rtv->cam.h_scalar,
				(rtv->column + anti_aliazing[sample][0] + 0.5) / WIN_HEIGHT),
				ft_scale_vector(rtv->cam.w_scalar,
				(rtv->row + anti_aliazing[sample][1] + 0.5) / WIN_WIDTH)));
	world_point = ft_sub_vector(world_point, rtv->cam.position);
	rtv->cam.ray_direction = ft_normalise_vector(world_point);
}

void		ft_init_cam(t_cam *cam)
{
	t_vector n_up;

	n_up = ft_new_vector(MIN_D, 1, MIN_D);
	n_up = ft_normalise_vector(ft_rotate_vector(n_up, cam->rotation));
	cam->position = ft_add_vector(cam->position, cam->translation);
	cam->foreward = ft_normalise_vector(ft_sub_vector(cam->look_at,
			cam->position));
	cam->right = ft_normalise_vector(
				ft_cross_product(cam->foreward, n_up));
	cam->up = ft_normalise_vector(ft_cross_product(cam->right,
			ft_scale_vector(cam->foreward, -1)));
	cam->ratio = (double)WIN_HEIGHT / (double)WIN_WIDTH;
	cam->half_height = tan(FT_RAD(cam->fov) / 2);
	cam->half_width = cam->half_height / cam->ratio;
	cam->bottom_left = ft_sub_vector(cam->position, ft_add_vector(
					ft_scale_vector(cam->up, cam->half_height),
					ft_scale_vector(cam->right, cam->half_width)));
	cam->bottom_left = ft_add_vector(cam->bottom_left, cam->foreward);
	cam->w_scalar = ft_scale_vector(cam->right, 2.0 * cam->half_width);
	cam->h_scalar = ft_scale_vector(cam->up, 2.0 * cam->half_height);
}
