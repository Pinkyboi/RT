/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_managing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 21:12:32 by azarzor           #+#    #+#             */
/*   Updated: 2020/01/12 17:07:49 by abenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void	ft_clear_mlx(t_mlx *mlx)
{
	mlx_destroy_image(mlx->mlx_ptr, mlx->img.img_ptr);
	mlx_clear_window(mlx->mlx_ptr, mlx->win);
	mlx->img.img_ptr = mlx_new_image(mlx->mlx_ptr, WIN_WIDTH, WIN_HEIGHT);
	mlx->img.data = (int*)mlx_get_data_addr(mlx->img.img_ptr, &mlx->img.bpp,
			&mlx->img.size_l, &mlx->img.endian);
}

int			ft_exit(t_rtv *rtv)
{
	(void)rtv;
	exit(0);
}

int			ft_key_stroke(int key, t_rtv *rtv)
{
	(key == EXIT) ? ft_exit(rtv) : 1;
	if (key == LEFT || key == RIGHT || key == UP ||
		key == DOWN || key == FOREWORD || key == BACKWARD)
	{
		rtv->actions.mouvement = 1;
		rtv->anti_aliasing = 0;
		rtv->render_offset = PIXEL_SIZE;
		rtv->render_y_offset = PIXEL_SIZE;
		rtv->pixel_size = PIXEL_SIZE;
		t_vector	move;

		move = ft_scale_vector(ft_normalise_vector(ft_sub_vector(rtv->cam.look_at,
			rtv->cam.position)), 3);
		(key == LEFT) ? rtv->cam.position.x -= 3 : 0;
		(key == RIGHT) ? rtv->cam.position.x += 3 : 0;
		(key == UP) ? rtv->cam.position.y += 3 : 0;
		(key == DOWN) ? rtv->cam.position.y -= 3 : 0;
		(key == FOREWORD) ? rtv->cam.position.z += 3 : 0;
		(key == BACKWARD) ? rtv->cam.position.z -= 3 : 0;
		ft_init_cam(&(rtv->cam));
	}
	return (0);
}


void		ft_put_pixel(t_rtv *rtv, int color)
{
	for (int i = 0; i < rtv->render_offset; i++)
	{
		for (int j = 0; j < rtv->render_y_offset; j++)
		{
			if (rtv->row >= 0 && rtv->row < WIN_WIDTH &&
				rtv->column >= 0 && rtv->column < WIN_HEIGHT)
				rtv->mlx.img.data[(int)(((int)(rtv->column - j) % WIN_HEIGHT) * WIN_WIDTH + (int)(rtv->row - i) % WIN_WIDTH)] = color;
		}
	}
	if (rtv->row >= 0 && rtv->row < WIN_WIDTH &&
		rtv->column >= 0 && rtv->column < WIN_HEIGHT)
		rtv->mlx.img.data[(int)(rtv->column * WIN_WIDTH + rtv->row)] = color;
}

int			ft_frame_loop(void *arg)
{
	t_rtv	*rtv;

	rtv = arg;
	if (rtv->render_offset >= 0 && rtv->render_y_offset >= 0)
		ft_ray_shooter(rtv);
	else if (rtv->anti_aliasing !=  rtv->scene.aa)
	{
		rtv->render_offset = 0;
		rtv->render_y_offset = 0;
		rtv->pixel_size = 1;
		rtv->anti_aliasing = rtv->scene.aa;
		ft_ray_shooter(rtv);
	}
	mlx_put_image_to_window(rtv->mlx.mlx_ptr, rtv->mlx.win,
			rtv->mlx.img.img_ptr, 0, 0);
	return (0);
}

void		ft_init_win(t_rtv *rtv)
{
	rtv->pixel_size = 1;
	rtv->render_offset = 0;
	rtv->anti_aliasing = rtv->scene.aa;
	rtv->render_y_offset = 0;
	rtv->actions.mouvement = 0;
	rtv->mlx.mlx_ptr = mlx_init();
	rtv->mlx.img.img_ptr = mlx_new_image(rtv->mlx.mlx_ptr,
			WIN_WIDTH, WIN_HEIGHT);
	rtv->mlx.img.data = (int*)mlx_get_data_addr(rtv->mlx.img.img_ptr,
			&rtv->mlx.img.bpp, &rtv->mlx.img.size_l, &rtv->mlx.img.endian);
	rtv->mlx.win = mlx_new_window(rtv->mlx.mlx_ptr, WIN_WIDTH,
			WIN_HEIGHT, "RTV1");
	ft_ray_shooter(rtv);
	mlx_hook(rtv->mlx.win, 2, 0, &ft_key_stroke, rtv);
	mlx_hook(rtv->mlx.win, 17, 1, (*ft_exit), &rtv);
	mlx_loop_hook(rtv->mlx.mlx_ptr, &ft_frame_loop, rtv);
	mlx_loop(rtv->mlx.mlx_ptr);
}