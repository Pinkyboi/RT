/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shapes_cut.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/18 02:09:05 by abenaiss          #+#    #+#             */
/*   Updated: 2020/02/15 09:24:21 by abenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void			ft_sphere_cut(t_rtv *env,
	t_xml_tag *tag, t_object *object, int *status)
{
	t_object	disk;
	t_vector	cut_center;
	double		new_radius;

	ft_define_limits(tag, &(object->sphere.limits), status);
	object->sphere.max_length = ft_clip_min(-1, ft_parse_float(
			ft_xml_get_value(tag, "length", "-1"), status));
	object->sphere.cut_orientation = ft_normalise_vector(ft_parse_vector(
			ft_xml_get_value(tag, "orientation", "(0,1,0)"), status));
	if (object->sphere.max_length > 0 &&
		object->sphere.max_length < 2 * object->sphere.radius)
	{
		cut_center = ft_add_vector(
		ft_scale_vector(object->sphere.cut_orientation, -object->sphere.radius
			+ object->sphere.max_length), object->sphere.center);
		new_radius = sqrt(FT_SQR(object->sphere.radius) -
			FT_SQR(ft_vector_size(ft_sub_vector(cut_center,
				object->sphere.center))));
		disk.plane = ft_define_plane(cut_center,
			object->sphere.cut_orientation, object, new_radius);
		disk.point.function = &ft_plane_intersection;
		disk.point.material = object->point.material;
		disk.plane.limits = object->sphere.limits;
		ft_object_push(env, disk, TYPE_PLANE);
	}
}

void			ft_demi_sphere_cut(t_rtv *env,
	t_xml_tag *tag, t_object *object, int *status)
{
	t_object	disk;
	t_vector	cut_center;
	double		new_radius;

	ft_define_limits(tag, &(object->sphere.limits), status);
	object->sphere.max_length = object->sphere.radius;
	object->sphere.cut_orientation = (t_vector){0, 1, 0};
	object->sphere.cut_orientation = ft_normalise_vector(ft_rotate_vector(
				object->sphere.cut_orientation, object->sphere.rotation));
	cut_center = ft_add_vector(
	ft_scale_vector(object->sphere.cut_orientation, -object->sphere.radius
		+ object->sphere.max_length), object->sphere.center);
	new_radius = sqrt(FT_SQR(object->sphere.radius) -
		FT_SQR(ft_vector_size(ft_sub_vector(cut_center,
			object->sphere.center))));
	disk.plane = ft_define_plane(cut_center,
		object->sphere.cut_orientation, object, new_radius);
	disk.point.function = &ft_plane_intersection;
	disk.point.material = object->point.material;
	disk.plane.limits = object->sphere.limits;
	ft_object_push(env, disk, TYPE_PLANE);
}

void			ft_cylinder_cut(t_rtv *env,
	t_xml_tag *tag, t_object *object, int *status)
{
	t_object disks[2];
	t_vector cut_center;

	ft_define_limits(tag, &(object->cylinder.limits), status);
	object->cylinder.max_length = ft_clip_min(-1,
		ft_parse_float(ft_xml_get_value(tag, "length", "-1"), status));
	object->cylinder.limit = ft_parse_vector(
		ft_xml_get_value(tag, "limit", "(0,0,0)"), status);
	if (object->cylinder.max_length > 0)
	{
		cut_center = ft_add_vector(ft_scale_vector(object->cylinder.axis,
			object->cylinder.max_length / 2), object->cylinder.center);
		disks[0].plane = ft_define_plane(cut_center, object->cylinder.axis,
			object, object->cylinder.radius);
		cut_center = ft_add_vector(ft_scale_vector(object->cylinder.axis,
			-object->cylinder.max_length / 2), object->cylinder.center);
		disks[1].plane = ft_define_plane(cut_center, object->cylinder.axis,
			object, object->cylinder.radius);
		disks[0].point.function = &ft_plane_intersection;
		disks[1].point.function = &ft_plane_intersection;
		disks[0].plane.limits = object->cylinder.limits;
		disks[1].plane.limits = object->cylinder.limits;
		ft_object_push(env, disks[0], TYPE_PLANE);
		ft_object_push(env, disks[1], TYPE_PLANE);
	}
}

void			ft_cone_cut(t_rtv *env,
	t_xml_tag *tag, t_object *object, int *status)
{
	t_object	disk;
	t_vector	cut_center;
	double		new_radius;

	ft_define_limits(tag, &(object->cone.limits), status);
	object->cone.max_length = ft_clip_min(-1, ft_parse_float(
		ft_xml_get_value(tag, "length", "-1"), status));
	object->cone.limit = ft_parse_vector(
		ft_xml_get_value(tag, "limit", "(0,0,0)"), status);
	if (object->cone.max_length > 0)
	{
		cut_center = ft_add_vector(ft_scale_vector(object->cone.axis,
			object->cone.max_length), object->cone.center);
		new_radius = tan(FT_RAD(object->cone.angle / 2))
			* object->cone.max_length;
		disk.plane = ft_define_plane(cut_center,
			object->cone.axis, object, new_radius);
		disk.point.function = &ft_plane_intersection;
		disk.point.material = object->point.material;
		disk.plane.limits = object->cone.limits;
		ft_object_push(env, disk, TYPE_PLANE);
	}
}
