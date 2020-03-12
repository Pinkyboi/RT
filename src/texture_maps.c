/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_maps.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 23:52:47 by abenaiss          #+#    #+#             */
/*   Updated: 2020/02/28 23:52:48 by abenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void	ft_bump_map(t_point *point, t_cam *cam)
{
	t_color		color;
	t_vector	bump;

	if (point->material.proced_bump)
		color = ft_get_procedural_texture_color(point->material.proced_bump,
		cam->hit.uv);
	else
		color = ft_get_texture_color(point->material.bump,
		cam->hit.uv, (t_color){0.5, 0.5, 0.5}, point->material.mode);
	bump = (t_vector){color.r - 0.5, color.g - 0.5, color.b - 0.5};
	cam->hit.normal.x += bump.x;
	cam->hit.normal.y += bump.y;
	cam->hit.normal.z += bump.z;
	cam->hit.normal = ft_normalise_vector(cam->hit.normal);
}

void	ft_transparency_map(t_point *point, t_cam *cam)
{
	t_color color;

	if (point->material.proced_transparency)
		color = ft_get_procedural_texture_color(
			point->material.proced_transparency,
			cam->hit.uv);
	else
		color = ft_get_texture_color(point->material.transparency,
		cam->hit.uv, (t_color){1 - point->material.transparency_index, 0, 0},
		point->material.mode);
	cam->hit.transparency = 1 - color.r;
}

void	ft_reflection_map(t_point *point, t_cam *cam)
{
	t_color color;

	if (point->material.proced_reflection)
		color = ft_get_procedural_texture_color(
			point->material.proced_reflection,
			cam->hit.uv);
	else
		color = ft_get_texture_color(point->material.reflection,
		cam->hit.uv, (t_color){point->material.reflection_index, 0, 0},
		point->material.mode);
	cam->hit.reflection = color.r;
}

void	ft_material_maps(t_point *point, t_cam *cam)
{
	if (point->material.proced_texture)
		cam->hit.color = ft_get_procedural_texture_color(
			point->material.proced_texture, cam->hit.uv);
	else if (point->material.texture)
		cam->hit.color = ft_get_texture_color(point->material.texture,
			cam->hit.uv, cam->hit.color, point->material.mode);
	if (point->material.bump || point->material.proced_bump)
		ft_bump_map(point, cam);
	if (point->material.transparency || point->material.proced_transparency)
		ft_transparency_map(point, cam);
	if (point->material.reflection || point->material.proced_reflection)
		ft_reflection_map(point, cam);
}
