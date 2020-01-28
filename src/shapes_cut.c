/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shapes_cut.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiri <abiri@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/18 02:09:05 by abenaiss          #+#    #+#             */
/*   Updated: 2020/01/27 20:01:29 by abiri            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void			ft_sphere_cut(t_rtv *env,
	t_xml_tag *tag, t_object *object, int *status)
{
	t_object	disk;
	t_vector	cut_center;
	double		new_radius;

	ft_bzero(&disk, sizeof(t_object));
	ft_define_limits(tag, &(object->sphere.limits), status);
	object->sphere.max_lenght = ft_clip_min(-1, ft_parse_float(
			ft_xml_get_value(tag, "lenght", "-1"), status));
	object->sphere.cut_orientation = ft_normalise_vector(ft_parse_vector(
			ft_xml_get_value(tag, "orientation", "(0,1,0)"), status));
	if (object->sphere.max_lenght > 0 &&
		object->sphere.max_lenght < 2 * object->sphere.radius)
	{
		cut_center = ft_add_vector(
		ft_scale_vector(object->sphere.cut_orientation, -object->sphere.radius
			+ object->sphere.max_lenght), object->sphere.center);
		new_radius = sqrt(FT_SQR(object->sphere.radius) -
			FT_SQR(ft_vector_size(ft_sub_vector(cut_center,
				object->sphere.center))));
		disk.plane = ft_define_plane(cut_center,
			object->sphere.cut_orientation, object->sphere.color, new_radius);
		disk.point.function = &ft_plane_intersection;
		ft_object_push(env, disk, TYPE_PLANE);
	}
}

void			ft_demi_sphere_cut(t_rtv *env,
	t_xml_tag *tag, t_object *object, int *status)
{
	t_object	disk;
	t_vector	cut_center;
	double		new_radius;

	ft_bzero(&disk, sizeof(t_object));
	ft_define_limits(tag, &(object->sphere.limits), status);
	object->sphere.max_lenght = object->sphere.radius;
	object->sphere.cut_orientation = (t_vector){0, 1, 0};
	object->sphere.cut_orientation = ft_normalise_vector(ft_rotate_vector(
				object->sphere.cut_orientation, object->sphere.rotation));
	cut_center = ft_add_vector(
	ft_scale_vector(object->sphere.cut_orientation, -object->sphere.radius
		+ object->sphere.max_lenght), object->sphere.center);
	new_radius = sqrt(FT_SQR(object->sphere.radius) -
		FT_SQR(ft_vector_size(ft_sub_vector(cut_center,
			object->sphere.center))));
	disk.plane = ft_define_plane(cut_center,
		object->sphere.cut_orientation, object->sphere.color, new_radius);
	disk.point.function = &ft_plane_intersection;
	ft_object_push(env, disk, TYPE_PLANE);
}

void			ft_cylinder_cut(t_rtv *env,
	t_xml_tag *tag, t_object *object, int *status)
{
	t_object upper_disk;
	t_object lower_disk;
	t_vector cut_center;

	ft_define_limits(tag, &(object->cylinder.limits), status);
	object->cylinder.max_lenght = ft_clip_min(-1,
		ft_parse_float(ft_xml_get_value(tag, "lenght", "-1"), status));
	object->cylinder.limit = ft_parse_vector(
		ft_xml_get_value(tag, "limit", "(0,0,0)"), status);
	if (object->cylinder.max_lenght > 0)
	{
		cut_center = ft_add_vector(ft_scale_vector(object->cylinder.axis,
			object->cylinder.max_lenght / 2), object->cylinder.center);
		upper_disk.plane = ft_define_plane(cut_center, object->cylinder.axis,
			object->cylinder.color, object->cylinder.radius);
		cut_center = ft_add_vector(ft_scale_vector(object->cylinder.axis,
			-object->cylinder.max_lenght / 2), object->cylinder.center);
		lower_disk.plane = ft_define_plane(cut_center, object->cylinder.axis,
			object->cylinder.color, object->cylinder.radius);
		upper_disk.point.function = &ft_plane_intersection;
		lower_disk.point.function = &ft_plane_intersection;
		upper_disk.point.texture = NULL;
		lower_disk.point.texture = NULL;
		upper_disk.point.bump = NULL;
		lower_disk.point.bump = NULL;
		ft_object_push(env, upper_disk, TYPE_PLANE);
		ft_object_push(env, lower_disk, TYPE_PLANE);
	}
}

void			ft_cone_cut(t_rtv *env,
	t_xml_tag *tag, t_object *object, int *status)
{
	t_object	disk;
	t_vector	cut_center;
	double		new_radius;

	ft_bzero(&disk, sizeof(t_object));
	ft_define_limits(tag, &(object->cone.limits), status);
	object->cone.max_lenght = ft_clip_min(-1, ft_parse_float(
		ft_xml_get_value(tag, "lenght", "-1"), status));
	object->cone.limit = ft_parse_vector(
		ft_xml_get_value(tag, "limit", "(0,0,0)"), status);
	if (object->cone.max_lenght > 0)
	{
		cut_center = ft_add_vector(ft_scale_vector(object->cone.axis,
			object->cone.max_lenght), object->cone.center);
		new_radius = tan(FT_RAD(object->cone.angle / 2))
			* object->cone.max_lenght;
		disk.plane = ft_define_plane(cut_center,
			object->cone.axis, object->cone.color, new_radius);
		disk.point.function = &ft_plane_intersection;
		ft_object_push(env, disk, TYPE_PLANE);
	}
}
