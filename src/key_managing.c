/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_managing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 21:12:32 by azarzor           #+#    #+#             */
/*   Updated: 2019/12/30 05:08:46 by abenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int			ft_exit(t_rtv *rtv)
{
	(void)rtv;
	exit(0);
}

int			ft_key_stroke(int key, void *test)
{
	t_rtv	*rtv;

	rtv = (t_rtv *)test;
	(key == 53) ? ft_exit(rtv) : 1;
	return (0);
}

void		ft_put_pixel(t_rtv *rtv, int color)
{
	if (rtv->row >= 0 && rtv->row < WIN_WIDTH &&
		rtv->column >= 0 && rtv->column <= WIN_HEIGHT)
		rtv->mlx.img.data[(int)(rtv->column * WIN_WIDTH + rtv->row)] = color;
}

void		ft_init_win(t_rtv *rtv)
{
	rtv->mlx.mlx_ptr = mlx_init();
	rtv->mlx.img.img_ptr = mlx_new_image(rtv->mlx.mlx_ptr,
			WIN_WIDTH, WIN_HEIGHT);
	rtv->mlx.img.data = (int*)mlx_get_data_addr(rtv->mlx.img.img_ptr,
			&rtv->mlx.img.bpp, &rtv->mlx.img.size_l, &rtv->mlx.img.endian);
	rtv->mlx.win = mlx_new_window(rtv->mlx.mlx_ptr, WIN_WIDTH,
			WIN_HEIGHT, "RTV1");
	ft_ray_shooter(rtv);
	mlx_put_image_to_window(rtv->mlx.mlx_ptr, rtv->mlx.win,
		rtv->mlx.img.img_ptr, 0, 0);
	mlx_hook(rtv->mlx.win, 2, 0, &ft_key_stroke, rtv);
	mlx_hook(rtv->mlx.win, 17, 1, (*ft_exit), &rtv);
	mlx_loop(rtv->mlx.mlx_ptr);
}
