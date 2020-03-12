/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_texture_color.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiri <abiri@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 23:58:19 by abenaiss          #+#    #+#             */
/*   Updated: 2020/03/04 23:21:27 by abiri            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int		ft_modulus(int value, int modulus)
{
	if (value < 0)
		return (modulus + value % modulus);
	else
		return (value % modulus);
}

t_color	ft_get_texture_color(t_texture *texture,
	t_coor uv, t_color original, int mode)
{
	u_int32_t	color;
	t_color		result;

	if (!(mode & TEXTURE_MODE_CENTER))
	{
		uv.x = ft_modulus((int)(uv.x * texture->width), texture->width);
		uv.y = ft_modulus((int)(uv.y * texture->height), texture->height);
	}
	else
	{
		uv.x = (int)(uv.x * texture->width);
		uv.y = (int)(uv.y * texture->height);
	}
	if (uv.y < 0 || uv.y >= texture->height
		|| uv.x < 0 || uv.x >= texture->width)
		return (original);
	color = texture->pixels[(int)uv.y * texture->width + (int)uv.x];
	result.r = (double)((color & 0xFF0000) >> 16) / 255.0;
	result.g = (double)((color & 0xFF00) >> 8) / 255.0;
	result.b = (double)(color & 0xFF) / 255.0;
	return (result);
}

t_color	ft_get_procedural_texture_color(t_procedural_texture_function *texture,
	t_coor uv)
{
	t_color		result;

	result = texture(uv.x, uv.y);
	return (result);
}

int		ft_get_texture_cut(t_material *material, t_coor uv)
{
	if (!material->texture)
		return (0);
	uv.x = (int)(uv.x * material->texture->width);
	uv.y = (int)(uv.y * material->texture->height);
	if (uv.x < 0 || uv.x >= material->texture->width
		|| uv.y < 0 || uv.y >= material->texture->height)
		return (1);
	return (0);
}

int		ft_is_transparent(t_material *material, t_coor uv)
{
	u_int32_t	color;
	t_texture	*texture;

	color = 0x0;
	texture = material->texture;
	if (texture)
	{
		if (!(material->mode & TEXTURE_MODE_CENTER))
		{
			uv.x = ft_modulus((int)(uv.x * texture->width), texture->width);
			uv.y = ft_modulus((int)(uv.y * texture->height), texture->height);
		}
		else
			uv = (t_coor){(int)(uv.x * texture->width),
				(int)(uv.y * texture->height)};
		if (uv.y < 0 || uv.y >= texture->height
			|| uv.x < 0 || uv.x >= texture->width)
			return (1);
		color = texture->pixels[(int)uv.y * texture->width + (int)uv.x];
	}
	else if (material->proced_transparency)
		color = ft_rgb_to_int(material->proced_transparency(uv.x, uv.y));
	if (!((color & 0xFF000000)))
		return (1);
	return (0);
}
