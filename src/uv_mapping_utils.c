/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uv_mapping_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiri <abiri@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/20 16:53:07 by azarzor           #+#    #+#             */
/*   Updated: 2020/02/23 03:33:29 by abiri            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_coor		ft_cart_to_sphere(t_vector vect, t_sphere *sphere)
{
	double		theta;
	double		phi;
	t_coor		mapped;

	theta = atan2(-(vect.z - sphere->center.z), vect.x - sphere->center.x);
	phi = acos(-(vect.y - sphere->center.y) / sphere->radius);
	mapped.x = ((theta + M_PI) / (2 * M_PI));
	mapped.y = (1 - phi / M_PI);
	mapped.x = (mapped.x * sphere->material.scale) + sphere->material.offset.x;
	mapped.y = (mapped.y * sphere->material.scale) + sphere->material.offset.y;
	return (mapped);
}

t_coor		ft_cart_to_cylinder(t_vector vect, t_cylinder *cylinder)
{
	double		theta;
	double		phi;
	t_coor		mapped;
	double		length;

	if (cylinder->max_lenght == -1)
		length = 10.0;
	else if (cylinder->max_lenght >= 0)
		length = cylinder->max_lenght;	
	theta = atan2(-(vect.z - cylinder->center.z), vect.x - cylinder->center.x);
	phi = (vect.y - cylinder->center.y ) / 20.0;
	
	mapped.y = (theta) / (2 * M_PI);
	mapped.x = phi;


	return(mapped);
}

t_coor		ft_cart_to_plane(t_cam *cam, t_plane *plane)
{
	t_coor		mapped;

	mapped.x = ft_dot_vector(
			ft_sub_vector(cam->hit.position, plane->center), plane->sides.u);
	mapped.y = ft_dot_vector(
			ft_sub_vector(cam->hit.position, plane->center), plane->sides.v);
	return (mapped);
}
