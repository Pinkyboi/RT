/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   basics_normals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiri <abiri@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/19 18:58:29 by abenaiss          #+#    #+#             */
/*   Updated: 2020/03/04 11:31:49 by abiri            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void	ft_get_hit_info(t_vector normal, t_point *point, t_cam *cam)
{
	if (cam->hit.soluces[0])
	{
		cam->hit.normal = normal;
		cam->hit.color = point->color;
		cam->hit.reflection = point->material.reflection_index;
		cam->hit.refraction = point->material.refraction_index;
		cam->hit.transparency = point->material.transparency_index;
		if ((point->material.mode & TEXTURE_MODE_CUT &&
			ft_get_texture_cut(&point->material, cam->hit.uv))
			|| (point->material.mode & TEXTURE_MODE_TRANSPARENCY &&
			ft_is_transparent(&point->material, cam->hit.uv)))
		{
			cam->hit.transparency = 1;
			return ;
		}
		ft_material_maps(point, cam);
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
	cam->hit.uv = ft_cart_to_sphere(cam->hit.position, sphere);
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
	cylinder->length = scale;
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
	cam->hit.uv = ft_cart_to_cone(cam->hit.position, cone);
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
	ft_intersection_position(cam, distance);
	cam->hit.uv = ft_cart_to_plane(cam->hit.position, plane);
	ft_get_hit_info(normal, (t_point*)plane, cam);
	cam->hit.normal = (i > 0) ?
		ft_scale_vector(plane->normal, -1) : plane->normal;
}
