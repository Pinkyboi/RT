/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   headless_rendering.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiri <abiri@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/20 15:45:31 by abiri             #+#    #+#             */
/*   Updated: 2020/03/04 14:12:57 by abiri            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int			ft_init_rendering_image(t_rtv *rtv)
{
	rtv->mlx.img.height = rtv->scene.height;
	rtv->mlx.img.width = rtv->scene.width;
	rtv->mlx.img.size_l = rtv->scene.width * 4;
	rtv->mlx.img.data = ft_memalloc(sizeof(int) *
		rtv->scene.width * rtv->scene.height);
	return (SUCCESS);
}

static void	ft_init_headless_renderer(t_rtv *rtv)
{
	rtv->anti_aliasing = 0;
	rtv->render_offset = 0;
	rtv->render_y_offset = 0;
	rtv->pixel_size = 1;
	rtv->anti_aliasing = rtv->scene.aa;
	rtv->actions.mouvement = 0;
	ft_memset(&rtv->options, 1, sizeof(rtv->options));
}

int			ft_headless_raytracer(t_rtv *rtv, char *filename)
{
	ft_init_headless_renderer(rtv);
	ft_init_rendering_image(rtv);
	ft_ray_shooter(rtv);
	ft_save_bitmap(&rtv->mlx.img, filename);
	return (SUCCESS);
}
