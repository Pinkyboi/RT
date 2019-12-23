/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normal_and_refraction.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/19 18:58:29 by abenaiss          #+#    #+#             */
/*   Updated: 2019/12/23 02:32:16 by abenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include <stdio.h>

void	ft_sphere_normal(t_cam *cam, t_sphere *sphere, double distance)
{
	t_vector	radius;
	t_vector	cut_center;

	ft_intersection_position(cam, distance);
	if(sphere->max_lenght > 0){
		
		cut_center = ft_add_vector(ft_scale_vector(
				sphere->cut_orientation, -sphere->radius + sphere->max_lenght),sphere->center);
		if((sphere->radius <= sphere->max_lenght) && ft_dot_vector(ft_add_vector(cut_center,sphere->cut_orientation),
			ft_sub_vector(cut_center, cam->intersection)) < 0)
				sphere->soluce[0] = 0;
		else if((sphere->radius > sphere->max_lenght) && ft_dot_vector(ft_add_vector(cut_center,sphere->cut_orientation),
			ft_sub_vector(cut_center, cam->intersection)) > 0)
				sphere->soluce[0] = 0;		
	}
	radius = ft_sub_vector(cam->intersection, sphere->center);
	sphere->normal = ft_normalise_vector(radius);
}

void	ft_cylinder_normal(t_cam *cam, t_cylinder *cylinder,
								double distance)
{
	t_vector	center_to_point;
	t_vector	scaled_axis;
	double		scale;

	
	ft_intersection_position(cam, distance);
	cylinder->axis = ft_normalise_vector(cylinder->axis);
	scale = ft_dot_vector(cam->ray_direction, cylinder->axis) * distance;
	scale += ft_dot_vector(ft_sub_vector(cam->position,
				cylinder->center), cylinder->axis);
	cylinder->lenght = scale;
	center_to_point = ft_sub_vector(cam->intersection, cylinder->center);
	scaled_axis = ft_scale_vector(cylinder->axis, scale);
	cylinder->normal = ft_normalise_vector(ft_sub_vector(center_to_point,
				scaled_axis));
}

void	ft_cone_normal(t_cam *cam, t_cone *cone, double distance)
{
	t_vector	center_to_point;
	t_vector	scaled_axis;
	double		scale;
	t_vector	cut_center;
			
	ft_intersection_position(cam, distance);
	if (cone->max_lenght > 0)
	{
		cut_center = ft_add_vector(ft_scale_vector(
			cone->axis, cone->max_lenght), cone->center);
		if(ft_dot_vector(ft_add_vector(cut_center,cone->axis),
			ft_sub_vector(cut_center, cam->intersection)) < 0)
			cone->soluce[0] = 0;
		else if(ft_dot_vector(ft_add_vector(cone->center,cone->axis),
			ft_sub_vector(cone->center, cam->intersection)) > 0)
			cone->soluce[0] = 0;
	}
	cone->axis = ft_normalise_vector(cone->axis);
	scale = ft_dot_vector(cam->ray_direction, cone->axis) * distance;
	scale += ft_dot_vector(ft_sub_vector(cam->position,
				cone->center), cone->axis);
	center_to_point = ft_sub_vector(cam->intersection, cone->center);
	scaled_axis = ft_scale_vector(ft_scale_vector(cone->axis, scale),
								(1 + FT_SQR(cone->tilt)));
	cone->normal = ft_normalise_vector(ft_sub_vector(center_to_point,
				scaled_axis));		

}

void	ft_refracted_ray(t_cam *cam, t_light *light, t_vector normal)
{
	t_vector	scaled_normal;
	double		normal_scalar;

	cam->reversed_ray = ft_normalise_vector(ft_sub_vector(cam->position,
				cam->intersection));
	light->light_vect = ft_normalise_vector(ft_sub_vector(light->center,
				cam->intersection));
	normal_scalar = 2.0 * ft_dot_vector(normal, light->light_vect);
	scaled_normal = ft_scale_vector(normal, normal_scalar);
	light->reflected_light_vect = ft_normalise_vector(ft_sub_vector(
		scaled_normal, light->light_vect));
}
