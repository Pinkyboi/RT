/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures_constructors.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiri <abiri@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 00:26:45 by abenaiss          #+#    #+#             */
/*   Updated: 2020/03/04 19:42:46 by abiri            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_texture						*ft_get_texture(char *filename, t_rtv *env)
{
	t_texture	*result;

	env->textures.iterator = env->textures.first;
	while ((result = ttslist_iter_content(&env->textures)))
	{
		if (result->texture_name && ft_strequ(filename, result->texture_name))
			return (result);
	}
	return (NULL);
}

t_texture						*ft_load_image(char *filename, t_rtv *rtv)
{
	t_texture	*result;
	int			fd;

	if ((fd = open(filename, O_RDONLY)) < 0)
		return (NULL);
	if (!(result = ft_memalloc(sizeof(t_texture))))
		return (NULL);
	if (read(fd, &result->width, 4) != 4)
		return (NULL);
	if (read(fd, &result->height, 4) != 4)
		return (NULL);
	if (!(result->pixels = malloc(sizeof(u_int32_t) *
		result->width * result->height)))
		return (NULL);
	read(fd, result->pixels, result->width *
		result->height * sizeof(u_int32_t));
	result->texture_name = filename;
	rtv->textures.push(&rtv->textures, result);
	return (result);
}

t_procedural_texture_function	*ft_load_procedural_texture(char *filename)
{
	if (ft_strequ(filename, "checker"))
		return (ft_checker_texture);
	if (ft_strequ(filename, "brick"))
		return (ft_brick_texture);
	if (ft_strequ(filename, "pastel"))
		return (ft_pastel);
	if (ft_strequ(filename, "marble"))
		return (ft_marble);
	if (ft_strequ(filename, "wood"))
		return (ft_wood);
	return (NULL);
}

t_texture						*ft_load_texture(char *filename, t_rtv *env,
	t_texture **texture, t_procedural_texture_function **function)
{
	t_texture	*result;

	if (!filename)
		return (NULL);
	if (filename[0] == ':')
	{
		*function = ft_load_procedural_texture(&filename[1]);
		return (NULL);
	}
	result = ft_get_texture(filename, env);
	if (!result)
		result = ft_load_image(filename, env);
	*texture = result;
	return (result);
}

int								ft_get_texture_mapping_type(t_xml_tag *tag)
{
	t_xml_prop	*prop;
	int			result;

	result = 0;
	prop = tag->props;
	while (prop)
	{
		if (ft_strequ(prop->name, "mapping_type"))
		{
			if (ft_strequ(prop->value, "fit"))
				result |= TEXTURE_MODE_FIT;
			else if (ft_strequ(prop->value, "center"))
				result |= TEXTURE_MODE_CENTER;
			else if (ft_strequ(prop->value, "cut"))
				result |= TEXTURE_MODE_CUT;
			else if (ft_strequ(prop->value, "transparency"))
				result |= TEXTURE_MODE_TRANSPARENCY;
			else if (ft_strequ(prop->value, "repeat"))
				result |= TEXTURE_MODE_REPEAT;
		}
		prop = prop->next;
	}
	return (result);
}
