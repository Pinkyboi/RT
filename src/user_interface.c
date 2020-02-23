/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user_interface.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiri <abiri@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/14 12:36:40 by abiri             #+#    #+#             */
/*   Updated: 2020/02/14 16:00:42 by abiri            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int			ft_button_toggle_boolean(void *arg, int status)
{
	unsigned int	*boolean;

	if (!arg)
		return (0);
	boolean = arg;
	if (status)
		*boolean = 0;
	else
		*boolean = 1;
	return (*boolean);
}

int			ft_button_render(void *arg, int status)
{
	t_rtv	*env;

	env = arg;
	env->render_offset = 0;
	env->render_y_offset = 0;
	env->pixel_size = 1;
	if (env->options.anti_aliasing)
		env->anti_aliasing = env->scene.aa;
	else
		env->anti_aliasing = 0;
	ft_ray_shooter(env);
	return (status);
}

int			ft_button_stereo(void *arg, int status)
{
	t_rtv	*env;

	env = arg;
	env->render_offset = 0;
	env->render_y_offset = 0;
	env->pixel_size = 1;
	if (env->options.anti_aliasing)
		env->anti_aliasing = env->scene.aa;
	else
		env->anti_aliasing = 0;
	ft_shoot_stero(env);
	return (status);
}

t_button	*ft_new_button(char *text, t_button_handler *handler,
	void *arg, t_coor position)
{
	t_button	*result;

	if (!(result = ft_memalloc(sizeof(t_button))))
		return (NULL);
	result->handler = handler;
	result->arg = arg;
	result->position = position;
	result->text = text;
	result->status = 0;
	return (result);
}

void		ft_activate_button(t_button *button, t_rtv *env)
{
	if (button->handler)
	{
		env->actions.mouvement = 1;
		env->anti_aliasing = 0;
		env->render_offset = PIXEL_SIZE;
		env->render_y_offset = PIXEL_SIZE;
		env->pixel_size = PIXEL_SIZE;
		button->status = button->handler(button->arg, button->status);
	}
}

int			ft_click_buttons(int mouse_button, int x, int y, t_rtv *env)
{
	t_button	*button;
	int			clicked;

	clicked = 0;
	(void)mouse_button;
	env->buttons.iterator = env->buttons.first;
	while ((button = ttslist_iter_content(&env->buttons)))
	{
		if (x > button->position.x && x < button->position.x + BUTTON_WIDTH &&
			y > button->position.y && y < button->position.y + BUTTON_HEIGHT)
				ft_activate_button(button, env);
	}
	return (0);
}

void		ft_draw_button(t_button *button, t_mlx *mlx_data)
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

void		ft_draw_buttons(t_rtv *env)
{
	t_button *button;

	env->buttons.iterator = env->buttons.first;
	while ((button = ttslist_iter_content(&env->buttons)))
		ft_draw_button(button, &env->mlx);
}

void		ft_load_interface(t_list_head *buttons, t_rtv *env)
{
	t_button	*button;

	button = ft_new_button(ft_strdup("AA"), ft_button_toggle_boolean,
		&env->options.anti_aliasing, (t_coor){0, 0});
	env->buttons.push(&env->buttons, button);
	button = ft_new_button(ft_strdup("DIFFUSE"), ft_button_toggle_boolean,
		&env->options.diffuse, (t_coor){0, 20});
	env->buttons.push(&env->buttons, button);
	button = ft_new_button(ft_strdup("SPECULAR"), ft_button_toggle_boolean,
		&env->options.specular, (t_coor){0, 40});
	env->buttons.push(&env->buttons, button);
	button = ft_new_button(ft_strdup("RENDER"), ft_button_render,
		env, (t_coor){0, 100});
	env->buttons.push(&env->buttons, button);
	button = ft_new_button(ft_strdup("STEREO"), ft_button_stereo,
		env, (t_coor){0, 120});
	env->buttons.push(&env->buttons, button);
}