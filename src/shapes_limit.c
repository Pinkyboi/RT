/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shapes_limit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiri <abiri@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/18 02:09:05 by abenaiss          #+#    #+#             */
/*   Updated: 2020/03/04 11:32:15 by abiri            ###   ########.fr       */
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
	double		max_length;

	max_length = sphere.max_length + MIN_D;
	if (sphere.max_length > 0)
	{
		cut_center = ft_add_vector(ft_scale_vector(sphere.cut_orientation,
			max_length - sphere.radius), sphere.center);
		cut_to_center = ft_normalise_vector(ft_sub_vector(sphere.center,
			cut_center));
		if ((sphere.radius <= max_length) &&
		ft_dot_vector(ft_sub_vector(cam.hit.position, cut_center),
			cut_to_center) < 0)
			return (0);
		if ((sphere.radius > max_length) &&
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

	if (cone.max_length > 0)
	{
		cut_center = ft_add_vector(ft_scale_vector(cone.axis,
			cone.max_length), cone.center);
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

	if (cylinder.max_length > 0)
	{
		cut_center[0] = ft_add_vector(ft_scale_vector(cylinder.axis,
			cylinder.max_length / 2), cylinder.center);
		cut_center[1] = ft_add_vector(ft_scale_vector(cylinder.axis,
			-cylinder.max_length / 2), cylinder.center);
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
	double		x[2];

	x[0] = ft_dot_vector(
			ft_sub_vector(cam.hit.position, plane.center), plane.sides.u);
	x[1] = ft_dot_vector(
			ft_sub_vector(cam.hit.position, plane.center), plane.sides.v);
	if (plane.radius < 0 &&
		((plane.length.u >= 0 && fabs(x[1]) >= plane.length.u)
		|| (plane.length.u >= 0 && fabs(x[0]) >= plane.length.v)))
		return (0);
	if (ft_axis_limit(cam.hit.position, plane.limits))
		return (0);
	return (cam.hit.soluces[0]);
}
