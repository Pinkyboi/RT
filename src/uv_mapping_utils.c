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

t_coor		ft_cart_to_cylinder(t_vector vect,
	t_cylinder *cylinder, t_vector scaled_axis)
{
	t_coor		mapped;
	t_vector	new_point;
	double		theta;

	new_point = ft_add_vector(vect, ft_scale_vector(scaled_axis, -1));
	theta = atan2(-(new_point.z - cylinder->center.z),
		new_point.x - cylinder->center.x);
	mapped.x = theta * cylinder->radius;
	mapped.y = ft_vector_size(scaled_axis);
	mapped.x = (mapped.x * cylinder->material.scale)
		+ cylinder->material.offset.x;
	mapped.y = (mapped.y * cylinder->material.scale)
		+ cylinder->material.offset.y;
	return (mapped);
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

t_coor		ft_cart_to_cone(t_vector vect, t_cone *cone)
{
	double		lenght;
	double		side_lenght;
	t_sphere	sphere;
	t_coor		mapped;

	if (cone->max_lenght < 0)
		lenght = 1000;
	else
		lenght = cone->max_lenght;
	side_lenght = sqrt(FT_SQR(lenght) + FT_SQR(lenght * cone->tilt));
	sphere.radius = ((side_lenght * lenght) /
	sqrt(FT_SQR(lenght) + FT_SQR(lenght)))
	/ (1 + (side_lenght) / sqrt(FT_SQR(lenght) + FT_SQR(lenght)));
	if (cone->max_lenght < 0)
		sphere.center = cone->center;
	else
		sphere.center = ft_add_vector(cone->center, ft_scale_vector(cone->axis,
			lenght - sphere.radius));
	sphere.material = cone->material;
	mapped = ft_cart_to_sphere(vect, &sphere);
	return (mapped);
}
