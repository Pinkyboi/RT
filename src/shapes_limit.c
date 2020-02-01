/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shapes_limit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/18 02:09:05 by abenaiss          #+#    #+#             */
/*   Updated: 2020/02/01 23:51:09 by abenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int		ft_axis_limit(t_vector intersection,
	t_limit limits)
{
	if (!(limits.x_limit.x < intersection.x
		&& intersection.x < limits.x_limit.y))
		return (1);
	if (!(limits.y_limit.x < intersection.y
		&& intersection.y < limits.y_limit.y))
		return (1);
	if (!(limits.z_limit.x < intersection.z
		&& intersection.z < limits.z_limit.y))
		return (1);
	return (0);
}

double	ft_sphere_limit(t_sphere sphere, t_cam cam)
{
	t_vector	cut_center;
	t_vector	cut_to_center;

	if (sphere.max_lenght > 0)
	{
		cut_center = ft_add_vector(ft_scale_vector(sphere.cut_orientation,
			sphere.max_lenght - sphere.radius), sphere.center);
		cut_to_center = ft_normalise_vector(ft_sub_vector(sphere.center,
			cut_center));
		if ((sphere.radius <= sphere.max_lenght) &&
		ft_dot_vector(ft_sub_vector(cam.hit.position, cut_center),
			cut_to_center) < 0)
			return (0);
		if ((sphere.radius > sphere.max_lenght) &&
			ft_dot_vector(ft_sub_vector(cam.hit.position, cut_center),
			cut_to_center) > 0)
			return (0);
	}
	if (ft_axis_limit(cam.hit.position, sphere.limits))
		return (0);
	return (cam.hit.soluces[0]);
}

double	ft_cone_limit(t_cone cone, t_cam cam)
{
	t_vector	cut_center;
	t_vector	cut_to_center;

	if (cone.max_lenght > 0)
	{
		cut_center = ft_add_vector(ft_scale_vector(cone.axis,
			cone.max_lenght), cone.center);
		cut_to_center = ft_normalise_vector(ft_sub_vector(cone.center,
			cut_center));
		if (ft_dot_vector(ft_sub_vector(cam.hit.position, cut_center),
			cut_to_center) < 0)
			return (0);
		if (ft_dot_vector(ft_sub_vector(cam.hit.position, cone.center),
			cone.axis) < 0)
			return (0);
	}
	if (ft_axis_limit(cam.hit.position, cone.limits))
		return (0);
	return (cam.hit.soluces[0]);
}

double	ft_cylinder_limit(t_cylinder cylinder, t_cam cam)
{
	t_vector	cut_center[2];
	t_vector	cut_to_center[2];

	if (cylinder.max_lenght > 0)
	{
		cut_center[0] = ft_add_vector(ft_scale_vector(cylinder.axis,
			cylinder.max_lenght / 2), cylinder.center);
		cut_center[1] = ft_add_vector(ft_scale_vector(cylinder.axis,
			-cylinder.max_lenght / 2), cylinder.center);
		cut_to_center[0] = ft_normalise_vector(ft_sub_vector(cylinder.center,
			cut_center[0]));
		cut_to_center[1] = ft_normalise_vector(ft_sub_vector(cylinder.center,
			cut_center[1]));
		if (ft_dot_vector(ft_sub_vector(cam.hit.position, cut_center[0]),
			cut_to_center[0]) < 0)
			return (0);
		if (ft_dot_vector(ft_sub_vector(cam.hit.position, cut_center[1]),
			cut_to_center[1]) < 0)
			return (0);
	}
	if (ft_axis_limit(cam.hit.position, cylinder.limits))
		return (0);
	return (cam.hit.soluces[0]);
}

double	ft_plane_limit(t_plane plane, t_cam cam)
{
	t_vector	up;
	t_vector	sides[2];
	double		x[2];

	up = ft_cross_product(ft_rotate_vector(plane.normal,
		(t_vector){90, 90, 90}), plane.normal);
	sides[0] = ft_normalise_vector(
			ft_cross_product(up, plane.normal));
	sides[1] = ft_normalise_vector(
			ft_cross_product(sides[0], plane.normal));
	x[0] = ft_dot_vector(
			ft_sub_vector(cam.hit.position, plane.center), sides[0]);
	x[1] = ft_dot_vector(
			ft_sub_vector(cam.hit.position, plane.center), sides[1]);
	if(fabs(x[0]) >= 50 || fabs(x[1]) >= 50)
		return (0);
	return (cam.hit.soluces[0]);
}