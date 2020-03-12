/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   slave_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merras <merras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/17 15:50:29 by abiri             #+#    #+#             */
/*   Updated: 2020/03/04 22:42:51 by merras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void			ft_init_default_camera(t_cam *cam)
{
	cam->position = (t_vector){0, 0, 0};
	cam->look_at = (t_vector){0, 0, -1};
	cam->fov = 65;
	cam->translation = (t_vector){0, 0, 0};
}

void			ft_init_default_scene(t_scene *scene)
{
	scene->ambiant = 0;
	scene->filter = 0;
	scene->aa = 0;
	scene->dof = 10;
	scene->dof_focus = 25;
	scene->reflection_depth = 3;
	scene->refraction_depth = 3;
	scene->width = 1.7777777778 * 720;
	scene->height = 720;
}

void			ft_init_rt(t_xml_data *data, t_rtv *rtv)
{
	ft_init_default_camera(&(rtv->cam));
	ft_init_default_scene(&(rtv->scene));
	ft_load_shapes(data, rtv);
	ft_init_cam(&rtv->cam, *rtv);
	ft_load_interface(&rtv->buttons, rtv);
	ft_headless_raytracer(rtv, "dump_file.bmp");
	while (1)
		continue;
}

int				main(void)
{
	t_rtv		rtv;
	t_xml_data	*data;
	char		*error;
	int			error_;

	srand(time(NULL));
	ft_bzero(&rtv, sizeof(t_rtv));
	ttslist_init(&rtv.textures);
	ttslist_init(&rtv.buttons);
	data = ft_read_xml(NULL);
	if (!data && (error_ = -2))
	{
		error = ft_xml_error(NULL, NULL);
		write(2, "PARSE ERROR : ", 14);
		if (error)
			write(2, error, ft_strlen(error));
		else
			ft_putstr_fd("Invalid file\n", 2);
		write(1, &error_, 4);
		return (-1);
	}
	error_ = -1;
	write(1, &error_, 4);
	ft_init_rt(data, &rtv);
	return (0);
}
