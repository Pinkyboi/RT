/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_managing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azarzor <azarzor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 21:12:32 by azarzor           #+#    #+#             */
/*   Updated: 2020/03/01 19:34:59 by azarzor          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static void	ft_move_cam(int key, t_rtv *rtv)
{
	static double	angles[3] = {0, 0, 0};

	if (key == RIGHT)
		angles[0] += M_PI / 36;
	if (key == LEFT)
		angles[0] -= M_PI / 36;
	if (key == UP)
		angles[1] += M_PI / 36;
	if (key == DOWN)
		angles[1] -= M_PI / 36;
	rtv->cam.look_at.x = 5 * cos(angles[0]);
	rtv->cam.look_at.z = 5 * sin(angles[0]);
	rtv->cam.look_at.y = 5 * sin(angles[1]);
	if (key == FOREWORD)
		rtv->cam.position = ft_add_vector(rtv->cam.position,
		ft_scale_vector(ft_normalise_vector(rtv->cam.look_at), 5));
	if (key == BACKWARD)
		rtv->cam.position = ft_sub_vector(rtv->cam.position,
		ft_scale_vector(ft_normalise_vector(rtv->cam.look_at), 5));
	rtv->cam.look_at = ft_add_vector(rtv->cam.position, rtv->cam.look_at);
	ft_init_cam(&rtv->cam, *rtv);
}

int			ft_key_stroke(int key, t_rtv *rtv)
{
	(key == EXIT) ? ft_exit(rtv) : 1;
	if (key == SAVE)
		ft_dump_bitmap(&rtv->mlx.img);
	if (key == LEFT || key == RIGHT || key == UP ||
		key == DOWN || key == FOREWORD || key == BACKWARD ||
		key == KEY_R || key == KEY_T)
	{
		rtv->actions.mouvement = 1;
		if (rtv->options.anti_aliasing)
			rtv->anti_aliasing = rtv->scene.aa;
		else
			rtv->anti_aliasing = 0;
		rtv->render_offset = PIXEL_SIZE;
		rtv->render_y_offset = PIXEL_SIZE;
		rtv->pixel_size = PIXEL_SIZE;
		(key == KEY_R) ? rtv->scene.dof_focus += 5 : 0;
		(key == KEY_T) ? rtv->scene.dof_focus -= 5 : 0;
		ft_move_cam(key, rtv);
	}
	return (0);
}
