/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_buttons.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiri <abiri@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 04:16:03 by abenaiss          #+#    #+#             */
/*   Updated: 2020/03/04 20:42:59 by abiri            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static void		ft_draw_button(t_button *button, t_mlx *mlx_data)
{
	int		x;
	int		y;
	int		index;
	t_color color;
	t_color	button_color;

	y = -1;
	while (++y < BUTTON_HEIGHT)
	{
		x = -1;
		while (++x < BUTTON_WIDTH)
		{
			index = (((int)button->position.y + y) % mlx_data->img.height) *
mlx_data->img.width + (x + (int)button->position.x) % mlx_data->img.width;
			color = ft_scale_colors(ft_int_to_rgb(
				mlx_data->img.data[index < 0 ? 0 : index]), 0.01);
			button_color = ((button->status) ? BUTTON_ACTIVE_COLOR :
				ft_add_colors(color, BUTTON_INACTIVE_COLOR));
			mlx_pixel_put(mlx_data->mlx_ptr, mlx_data->win, x +
button->position.x, y + button->position.y, ft_rgb_to_int(button_color));
		}
	}
	if (button->text)
		mlx_string_put(mlx_data->mlx_ptr, mlx_data->win, button->position.x,
			button->position.y, BUTTON_TEXT_COLOR, button->text);
}

void			ft_draw_buttons(t_rtv *env)
{
	t_button *button;

	env->buttons.iterator = env->buttons.first;
	while ((button = ttslist_iter_content(&env->buttons)))
		ft_draw_button(button, &env->mlx);
}
