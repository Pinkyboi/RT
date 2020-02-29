/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_buttons.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 04:16:03 by abenaiss          #+#    #+#             */
/*   Updated: 2020/02/29 04:16:04 by abenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static void		ft_draw_button(t_button *button, t_mlx *mlx_data)
{
	int	x;
	int	y;

	y = 0;
	while (y < BUTTON_HEIGHT)
	{
		x = 0;
		while (x < BUTTON_WIDTH)
		{
			mlx_pixel_put(mlx_data->mlx_ptr, mlx_data->win,
			x + button->position.x, y + button->position.y,
			(button->status) ? BUTTON_ACTIVE_COLOR : BUTTON_INACTIVE_COLOR);
			x++;
		}
		y++;
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
