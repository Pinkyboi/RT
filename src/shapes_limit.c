/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shapes_limit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/18 02:09:05 by abenaiss          #+#    #+#             */
/*   Updated: 2019/12/23 07:41:41 by abenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void	ft_sphere_limit(t_sphere *sphere, t_cam cam)
{
	t_vector	cut_center;

	if (sphere->max_lenght > 0)
	{
		cut_center = ft_add_vector(ft_scale_vector(sphere->cut_orientation,
			-sphere->radius + sphere->max_lenght), sphere->center);
		if ((sphere->radius <= sphere->max_lenght) &&
			ft_dot_vector(ft_add_vector(cut_center, sphere->cut_orientation),
				ft_sub_vector(cut_center, cam.intersection)) < 0)
			sphere->soluce[0] = 0;
		else if ((sphere->radius > sphere->max_lenght) &&
			ft_dot_vector(ft_add_vector(cut_center, sphere->cut_orientation),
				ft_sub_vector(cut_center, cam.intersection)) > 0)
			sphere->soluce[0] = 0;
	}
}

void	ft_cone_limit(t_cone *cone, t_cam cam)
{
	t_vector	cut_center;

	if (cone->max_lenght > 0)
	{
		cut_center = ft_add_vector(ft_scale_vector(
			cone->axis, cone->max_lenght), cone->center);
		if (ft_dot_vector(ft_add_vector(cut_center, cone->axis),
			ft_sub_vector(cut_center, cam.intersection)) < 0)
			cone->soluce[0] = 0;
		else if (ft_dot_vector(ft_add_vector(cone->center, cone->axis),
			ft_sub_vector(cone->center, cam.intersection)) > 0)
			cone->soluce[0] = 0;
	}
}

void	ft_cylinder_limit(t_cylinder *cylinder)
{
	if (cylinder->max_lenght > 0)
	{
		if (fabs(cylinder->lenght) > cylinder->max_lenght / 2)
			cylinder->soluce[0] = 0;
	}
}
