/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uv_mapping_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/20 16:53:07 by azarzor           #+#    #+#             */
/*   Updated: 2020/02/21 23:20:38 by abenaiss         ###   ########.fr       */
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

// t_coor		ft_cart_to_cylinder(t_vector vect, t_cylinder *cylinder)
// {
// 	double	theta;
// 	t_vector relative_center;
// 	t_vector u,v;

// 	relative_center = ;
// 	u = (t_vector){1, 0, 0};
// 	v = (t_vector){0, 0, 1};
// 	theta = atan2(vect.z - v.z, vect.x - u.x);
// 	return (mapped);
// }

t_coor		ft_cart_to_cylinder(t_vector vect, t_cylinder *cylinder, t_vector scaled_axis)
{
	t_vector u,v;
	t_vector relative_u;
	t_vector relative_center;
	t_coor		mapped;

	u = (t_vector){1, 0, 0};
	// v = (t_vector){0, 0, 1};
	relative_center = ft_add_vector(cylinder->center, scaled_axis);
	relative_u = ft_add_vector(ft_add_vector
		(cylinder->center, ft_scale_vector(u, cylinder->radius)),scaled_axis);
	double theta = acos(ft_dot_vector(relative_u,
		ft_normalise_vector(ft_sub_vector(relative_center, vect))));
	mapped.x = theta * 2 * cylinder->radius / M_PI;
	mapped.y = ft_vector_size(scaled_axis);
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
