/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   basics_normals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/19 18:58:29 by abenaiss          #+#    #+#             */
/*   Updated: 2020/02/21 22:55:56 by abenaiss         ###   ########.fr       */
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

int		ft_get_texture_cut(t_material *material, t_coor uv)
{
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

	texture = material->texture;
	if (!(material->mode & TEXTURE_MODE_CENTER))
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
		return (1);
	color = texture->pixels[(int)uv.y * texture->width + (int)uv.x];
	if (!((color & 0xFF000000) >> 24))
		return (1);
	return (0);
}

void	ft_get_hit_info(t_vector normal, t_point *point, t_cam *cam)
{
	t_vector	bump;
	t_color		color;

	if (cam->hit.soluces[0])
	{
		cam->hit.normal = normal;
		cam->hit.color = point->color;
		cam->hit.reflection = point->reflection;
		cam->hit.refraction = point->refraction;
		cam->hit.transparency = point->transparency;
		if ((point->material.mode & TEXTURE_MODE_CUT &&
			ft_get_texture_cut(&point->material, cam->hit.uv))
			|| (point->material.mode & TEXTURE_MODE_TRANSPARENCY &&
			ft_is_transparent(&point->material, cam->hit.uv)))
		{
			cam->hit.transparency = 1;
			return ;
		}
		if (point->material.texture)
			cam->hit.color = ft_get_texture_color(point->material.texture,
				cam->hit.uv, cam->hit.color, point->material.mode);
		if (point->material.bump)
		{
			color = ft_get_texture_color(point->material.bump,
				cam->hit.uv, (t_color){0.5, 0.5, 0.5}, point->material.mode);
			bump = (t_vector){color.r - 0.5, color.g - 0.5, color.b - 0.5};
			cam->hit.normal.x += bump.x;
			cam->hit.normal.y += bump.y;
			cam->hit.normal.z += bump.z;
			cam->hit.normal = ft_normalise_vector(cam->hit.normal);
		}
		if (point->material.transparency)
		{
			color = ft_get_texture_color(point->material.transparency,
				cam->hit.uv, (t_color){1 - point->transparency, 0, 0},
				point->material.mode);
			cam->hit.transparency = 1 - color.r;
		}
		if (point->material.reflection)
		{
			color = ft_get_texture_color(point->material.reflection,
			cam->hit.uv, (t_color){point->reflection, 0, 0},
			point->material.mode);
			cam->hit.reflection = color.r;
		}
	}
}

void	ft_sphere_normal(t_cam *cam, t_sphere *sphere, double distance)
{
	t_vector	radius;
	t_vector	normal;

	ft_intersection_position(cam, distance);
	cam->hit.soluces[0] = ft_sphere_limit(*sphere, *cam);
	if (!cam->hit.soluces[0])
		return ;
	radius = ft_sub_vector(cam->hit.position, sphere->center);
	normal = ft_normalise_vector(radius);
	ft_get_hit_info(normal, (t_point *)sphere, cam);
}

void	ft_cylinder_normal(t_cam *cam, t_cylinder *cylinder,
	double distance)
{
	t_vector	center_to_point;
	t_vector	scaled_axis;
	double		scale;
	t_vector	normal;
	t_cam		temp_cam;

	cylinder->axis = ft_normalise_vector(cylinder->axis);
	temp_cam = *cam;
	scale = ft_dot_vector(cam->ray_direction, cylinder->axis) * distance;
	scale += ft_dot_vector(ft_sub_vector(cam->position,
				cylinder->center), cylinder->axis);
	cylinder->lenght = scale;
	ft_intersection_position(&temp_cam, distance);
	cam->hit.soluces[0] = ft_cylinder_limit(*cylinder, temp_cam);
	if (!cam->hit.soluces[0])
		return ;
	ft_intersection_position(cam, distance);
	center_to_point = ft_sub_vector(cam->hit.position, cylinder->center);
	scaled_axis = ft_scale_vector(cylinder->axis, scale);
	normal = ft_normalise_vector(ft_sub_vector(center_to_point,
					scaled_axis));
	cam->hit.uv = ft_cart_to_cylinder(cam->hit.position, cylinder, scaled_axis);
	ft_get_hit_info(normal, (t_point *)cylinder, cam);
}

void	ft_cone_normal(t_cam *cam, t_cone *cone, double distance)
{
	t_vector	center_to_point;
	t_vector	scaled_axis;
	double		scale;
	t_vector	normal;
	t_cam		temp_cam;

	temp_cam = *cam;
	ft_intersection_position(&temp_cam, distance);
	cam->hit.soluces[0] = ft_cone_limit(*cone, temp_cam);
	if (!cam->hit.soluces[0])
		return ;
	ft_intersection_position(cam, distance);
	cone->axis = ft_normalise_vector(cone->axis);
	scale = ft_dot_vector(cam->ray_direction, cone->axis) * distance;
	scale += ft_dot_vector(ft_sub_vector(cam->position,
				cone->center), cone->axis);
	center_to_point = ft_sub_vector(cam->hit.position, cone->center);
	scaled_axis = ft_scale_vector(ft_scale_vector(cone->axis, scale),
								(1 + FT_SQR(cone->tilt)));
	normal = ft_normalise_vector(ft_sub_vector(center_to_point,
					scaled_axis));
	ft_get_hit_info(normal, (t_point *)cone, cam);
}

void	ft_plane_normal(t_cam *cam, t_plane *plane, double i)
{
	t_vector	normal;
	t_cam		clone;
	double		distance;

	normal = plane->normal;
	clone = *cam;
	distance = cam->hit.soluces[0];
	ft_intersection_position(&clone, distance);
	if (!(plane->radius < 0 || plane->radius >=
		ft_vector_size(ft_sub_vector(plane->center, clone.hit.position))))
	{
		cam->hit.soluces[0] = 0;
		return ;
	}
	cam->hit.soluces[0] = ft_plane_limit(*plane, clone);
	if (!cam->hit.soluces[0])
		return ;
	cam->hit.normal = (i > 0) ?
		ft_scale_vector(normal, -1) : normal;
	ft_intersection_position(cam, distance);
	ft_get_hit_info(normal, (t_point*)plane, cam);
}
