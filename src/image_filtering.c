/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image_filtering.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/15 08:24:31 by abenaiss          #+#    #+#             */
/*   Updated: 2020/02/15 08:24:39 by abenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static double g_motion_blur[9][9] =
{
	{1, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 1, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 1, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 1, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 1, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 1, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 1, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 1, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 1},
};

static double g_blur[9][9] =
{
	{0, 0, 1, 0, 0, 0, 0, 0, 0},
	{0, 1, 1, 1, 0, 0, 0, 0, 0},
	{1, 1, 1, 1, 1, 0, 0, 0, 0},
	{0, 1, 1, 1, 0, 0, 0, 0, 0},
	{0, 0, 1, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0},
};

static double g_sharp[9][9] =
{
	{-1, -1, -1, -1, -1, 0, 0, 0, 0},
	{-1, 2, 2, 2, -1, 0, 0, 0, 0},
	{-1, 2, 8, 2, -1, 0, 0, 0, 0},
	{-1, 2, 2, 2, -1, 0, 0, 0, 0},
	{-1, -1, -1, -1, -1, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0},
};

static t_color	ft_filtring_calcul(t_rtv *rtv, double
		filter_matrix[9][9], t_coor matrix_prop, t_coor map_coor)
{
	t_coor	new_image;
	t_color	color[2];
	t_coor	filter_coor;

	NEW_COLOR = (t_color){0.0, 0.0, 0.0};
	filter_coor.x = -1;
	while (++filter_coor.x < matrix_prop.x)
	{
		filter_coor.y = -1;
		while (++filter_coor.y < matrix_prop.y)
		{
			new_image.x = (int)map_coor.x - (int)matrix_prop.x /
				2 + (int)filter_coor.x;
			new_image.y = (int)map_coor.y - (int)matrix_prop.y /
				2 + (int)filter_coor.y;
			OLD_COLOR = ft_scale_colors(ft_int_to_rgb(rtv->mlx.img.data
				[(int)(new_image.x +
				rtv->scene.width * new_image.y)]), 1.0 / 255.0);
			if (new_image.x > 0 && new_image.x < rtv->scene.width &&
				new_image.y > 0 && new_image.y < rtv->scene.height)
				NEW_COLOR = ft_add_colors(ft_scale_colors(OLD_COLOR,
			filter_matrix[(int)filter_coor.x][(int)filter_coor.y]), NEW_COLOR);
		}
	}
	return (NEW_COLOR);
}

static void		ft_image_filtring(t_rtv *rtv, double
		filter_matrix[9][9], t_coor matrix_prop, double factor)
{
	t_color	newcolor;
	int		x;
	int		y;

	x = -1;
	while (++x < rtv->scene.width)
	{
		y = -1;
		while (++y < rtv->scene.height)
		{
			newcolor = ft_filtring_calcul(rtv,
				filter_matrix, matrix_prop, (t_coor){x, y});
			newcolor = ft_scale_colors(newcolor, 1.0 / factor);
			rtv->mlx.img.data[x +
				rtv->scene.width * y] = ft_rgb_to_int(newcolor);
		}
	}
}

void			ft_filtring_select(t_rtv *rtv)
{
	if (rtv->scene.effect == 1)
		ft_image_filtring(rtv, g_motion_blur, (t_coor){9, 9}, 9.0);
	if (rtv->scene.effect == 2)
		ft_image_filtring(rtv, g_blur, (t_coor){5, 5}, 13.0);
	if (rtv->scene.effect == 3)
		ft_image_filtring(rtv, g_sharp, (t_coor){5, 5}, 8.0);
}
