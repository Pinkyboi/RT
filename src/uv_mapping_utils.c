/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uv_mapping_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azarzor <azarzor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/20 16:53:07 by azarzor           #+#    #+#             */
/*   Updated: 2020/01/25 20:27:38 by azarzor          ###   ########.fr       */
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
	mapped.y = phi / M_PI;

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