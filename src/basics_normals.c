/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   basics_normals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiri <abiri@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/19 18:58:29 by abenaiss          #+#    #+#             */
/*   Updated: 2020/01/28 18:55:40 by abiri            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_color	ft_get_texture_color(t_texture *texture, t_coor uv)
{
	u_int32_t	color;
	t_color		result;

	uv.x = abs((int)(uv.x * texture->width) % texture->width);
	uv.y = abs((int)(uv.y * texture->height) % texture->height);
	color = texture->pixels[(int)uv.y * texture->width + (int)uv.x];
	result.r = (double)((color & 0xFF0000) >> 16) / 255.0;
	result.g = (double)((color & 0xFF00) >> 8) / 255.0;
	result.b = (double)(color & 0xFF) / 255.0;
	return (result);
}

void	ft_get_hit_info(t_vector normal, t_point *point, t_cam *cam)
{
	t_vector	bump;
	t_color		color;

	if (cam->hit.soluces[0])
	{
		cam->hit.normal = normal;
		cam->hit.color = point->color;
		if (point->texture)
			cam->hit.color = ft_get_texture_color(point->texture, cam->hit.uv);
		if (point->bump)
		{
			color = ft_get_texture_color(point->bump, cam->hit.uv);
			bump = (t_vector){color.r, color.g, color.b};
			cam->hit.normal.x *= bump.x;
			cam->hit.normal.y *= bump.y;
			cam->hit.normal.z *= bump.z;
			cam->hit.normal = ft_normalise_vector(cam->hit.normal);
		}
		cam->hit.reflection = point->reflection;
		cam->hit.refraction = point->refraction;
		cam->hit.transparency = point->transparency;
		cam->hit.uv = (t_coor){0, 0};
	}
}

void	ft_sphere_normal(t_cam *cam, t_sphere *sphere, double distance)
{
	t_vector	radius;
	t_vector	normal;

	ft_intersection_position(cam, distance);
	cam->hit.uv = ft_cart_to_sphere(cam->hit.position, sphere);
	cam->hit.soluces[0] = ft_sphere_limit(*sphere, *cam);
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

	ft_intersection_position(cam, distance);
	cylinder->axis = ft_normalise_vector(cylinder->axis);
	scale = ft_dot_vector(cam->ray_direction, cylinder->axis) * distance;
	scale += ft_dot_vector(ft_sub_vector(cam->ray_origin,
				cylinder->center), cylinder->axis);
	cylinder->lenght = scale;
	cam->hit.soluces[0] = ft_cylinder_limit(*cylinder, *cam);
	center_to_point = ft_sub_vector(cam->hit.position, cylinder->center);
	scaled_axis = ft_scale_vector(cylinder->axis, scale);
	normal = ft_normalise_vector(ft_sub_vector(center_to_point,
					scaled_axis));
	ft_get_hit_info(normal, (t_point *)cylinder, cam);
}

void	ft_cone_normal(t_cam *cam, t_cone *cone, double distance)
{
	t_vector	center_to_point;
	t_vector	scaled_axis;
	double		scale;
	t_vector	normal;

	ft_intersection_position(cam, distance);
	cam->hit.soluces[0] = ft_cone_limit(*cone, *cam);
	cone->axis = ft_normalise_vector(cone->axis);
	scale = ft_dot_vector(cam->ray_direction, cone->axis) * distance;
	scale += ft_dot_vector(ft_sub_vector(cam->ray_origin,
				cone->center), cone->axis);
	center_to_point = ft_sub_vector(cam->hit.position, cone->center);
	scaled_axis = ft_scale_vector(ft_scale_vector(cone->axis, scale),
								(1 + FT_SQR(cone->tilt)));
	normal = ft_normalise_vector(ft_sub_vector(center_to_point,
					scaled_axis));
	ft_get_hit_info(normal, (t_point *)cone, cam);
}
