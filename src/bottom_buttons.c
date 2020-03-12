/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bottom_buttons.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiri <abiri@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 04:05:06 by abenaiss          #+#    #+#             */
/*   Updated: 2020/03/04 11:37:23 by abiri            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static int			ft_button_re_render(void *arg, int status, t_rtv *env)
{
	(void)arg;
	env->render_offset = 0;
	env->render_y_offset = 0;
	env->pixel_size = 1;
	if (env->options.anti_aliasing)
		env->anti_aliasing = env->scene.aa;
	ft_ray_shooter(env);
	ft_filtring_select(env);
	return (status);
}

static int			ft_button_stereo(void *arg, int status, t_rtv *env)
{
	(void)arg;
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

static int			ft_button_save(void *arg, int status, t_rtv *env)
{
	(void)arg;
	ft_dump_bitmap(&env->mlx.img);
	return (status);
}

void				ft_bottom_buttons(t_list_head *buttons, t_rtv *env)
{
	t_button	*button;

	button = ft_new_button(ft_strdup("RENDER"), ft_button_re_render,
		env, (t_coor){0, env->scene.height - 20});
	buttons->push(buttons, button);
	button = ft_new_button(ft_strdup("SAVE"), ft_button_save,
		env, (t_coor){env->scene.width - 100, env->scene.height - 20});
	buttons->push(buttons, button);
	button = ft_new_button(ft_strdup("STEREO"), ft_button_stereo,
		env, (t_coor){env->scene.width / 2 - 50, env->scene.height - 20});
	buttons->push(buttons, button);
}
