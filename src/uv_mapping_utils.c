/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uv_mapping_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiri <abiri@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/20 16:53:07 by azarzor           #+#    #+#             */
/*   Updated: 2020/01/28 18:56:23 by abiri            ###   ########.fr       */
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
	mapped.x = (theta + M_PI) / (2 * M_PI);
	mapped.y = 1 - phi / M_PI;
	return(mapped);
}

t_coor		ft_cart_to_cylinder(t_vector vect, t_cylinder *cylinder)
{
	double		phi;
	double		r;
	t_coor		mapped;

	phi = atan2((vect.y - cylinder->center.y), vect.x - cylinder->center.x);
	mapped.x = phi / (2 * M_PI);
	mapped.y = ((vect.z - cylinder->center.z) + 1);
	return(mapped);
}

t_coor		ft_cart_to_plane(t_cam *cam, t_plane *plane)
{
	t_vector	sides[2];
	t_vector	up;
	t_coor		uv;

	up = ft_cross_product(ft_rotate_vector(plane->normal,
		(t_vector){90, 90, 90}), plane->normal);
	sides[0] = ft_normalise_vector(
			ft_cross_product(up, plane->normal));
	sides[1] = ft_normalise_vector(
			ft_cross_product(sides[0], plane->normal));
	uv.x = ft_dot_vector(
			ft_sub_vector(cam->hit.position, plane->center), sides[0]);
	uv.y = ft_dot_vector(
			ft_sub_vector(cam->hit.position, plane->center), sides[1]);
	uv.x /= 500;
	uv.y /= 500;
	return (uv);
}
