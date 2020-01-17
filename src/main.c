/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/17 15:50:29 by abiri             #+#    #+#             */
/*   Updated: 2020/01/16 22:20:50 by abenaiss         ###   ########.fr       */
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
	scene->reflection_depth = 3;
	scene->refraction_depth = 3;
	scene->width = 1.7777777778 * 720;
	scene->width = 720;
}

int				main(int argc, char **argv)
{
	t_rtv		rtv;
	t_xml_data	*data;
	char		*error;

	if (argc != 2)
		return (-1);
	ft_bzero(&rtv, sizeof(t_rtv));
	data = ft_read_xml(argv[1]);
	if (!data)
	{
		error = ft_xml_error(NULL, NULL);
		write(2, "PARSE ERROR : ", 14);
		if (error)
			write(2, error, ft_strlen(error));
		else
			ft_putstr_fd("Invalid file", 2);
		write(2, "\n", 1);
		return (-1);
	}
	ft_init_default_camera(&(rtv.cam));
	ft_init_default_scene(&(rtv.scene));
	ft_load_shapes(data, &rtv);
	ft_init_cam(&rtv);
	ft_init_win(&rtv);
	return (0);
}
