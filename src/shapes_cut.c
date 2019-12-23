/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shapes_cut.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/18 02:09:05 by abiri             #+#    #+#             */
/*   Updated: 2019/12/23 02:23:34 by abenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void			ft_print_vect(t_vector vector, char *name)
{
	printf("%s", name);
	printf(":(%f, %f, %f)\n", vector.x,vector.y,vector.z);
}

t_plane			ft_define_plane(t_vector center, t_vector normal, t_color color, double radius)
{
	t_plane plane;

	plane.center = center;
	plane.normal = normal;
	plane.color	 = color;
	plane.radius = radius;
	return (plane);
}

void            ft_sphere_cut(t_rtv *env, t_object object)
{
    t_object	disk;
	t_vector	cut_center;
	double		new_radius;

    if(object.sphere.max_lenght > 0 && object.sphere.max_lenght < 2 * object.sphere.radius)
	{
		cut_center = ft_add_vector(ft_scale_vector(object.sphere.cut_orientation,
			- object.sphere.radius + object.sphere.max_lenght),object.sphere.center);
		new_radius = sqrt(FT_SQR(object.sphere.radius) -
			FT_SQR(ft_vector_size(ft_sub_vector(cut_center, object.sphere.center))));
		disk.plane = ft_define_plane(cut_center, 
			object.sphere.cut_orientation, object.sphere.color,new_radius);
		disk.point.function = &ft_plane_intersection;
		ft_object_push(env, disk, TYPE_PLANE);
	}
}

void			ft_cylinder_cut(t_rtv *env, t_object object)
{
	t_object upper_disk;
	t_object lower_disk;
	t_vector cut_center;

    if(object.cylinder.max_lenght > 0)
    {
        cut_center = ft_add_vector(ft_scale_vector(object.cylinder.axis,
                    object.cylinder.max_lenght/2),object.cylinder.center);
        upper_disk.plane = ft_define_plane(cut_center, 
                object.cylinder.axis, object.cylinder.color,object.cylinder.radius);
        cut_center = ft_add_vector(ft_scale_vector(object.cylinder.axis,
                    -object.cylinder.max_lenght/2),object.cylinder.center);
        lower_disk.plane = ft_define_plane(cut_center, 
                object.cylinder.axis, object.cylinder.color,object.cylinder.radius);
        upper_disk.point.function = &ft_plane_intersection;
        lower_disk.point.function = &ft_plane_intersection;
        ft_object_push(env, upper_disk, TYPE_PLANE);
        ft_object_push(env, lower_disk, TYPE_PLANE);        
    }
}

void			ft_cone_cut(t_rtv *env, t_object object)
{
	t_object	disk;
	t_vector	cut_center;
	double		new_radius;

	if(object.cone.max_lenght > 0)
    {
		cut_center = ft_add_vector(ft_scale_vector(object.cone.axis,
            object.cone.max_lenght),object.cone.center);
		new_radius = tan(FT_RAD(object.cone.angle / 2)) * object.cone.max_lenght;
		disk.plane = ft_define_plane(cut_center, 
		object.cone.axis, object.cone.color,new_radius);
		disk.point.function = &ft_plane_intersection;
		ft_object_push(env, disk, TYPE_PLANE);
	}
}