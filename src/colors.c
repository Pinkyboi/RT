/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 22:04:25 by azarzor           #+#    #+#             */
/*   Updated: 2020/01/07 16:28:12 by abenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

double			ft_check_shadow(t_rtv *rtv, t_light light,
					t_vector normal, t_color *color)
{
	double intersection_dist;
	double dot;

	intersection_dist = 0.0;
	light.center = ft_add_vector(light.center, ft_scale_vector(normal, MIN_D));
	intersection_dist = ft_check_intersection(*rtv,
	light.light_vect, rtv->cam.intersection);
	dot = ft_dot_vector(light.light_vect, normal);
	if ((intersection_dist = ft_check_intersection(*rtv,
		light.light_vect, rtv->cam.intersection)))
	{
		if ((intersection_dist > ft_vector_size(ft_sub_vector(light.center
			, rtv->cam.intersection))) || dot < 0)
			return (1);
		if (dot >= 0)
			return (0);
		else
		{
			*color = ft_add_colors(*color, ft_scale_colors(*color,
			ft_calculate_shadow(*rtv, intersection_dist, light)));
			return (0);
		}
	}
	return (1);
}

t_color			ft_mix_colors(t_rtv *rtv, t_vector normal, t_color color)
{
	t_color			dif_col;
	t_color			spec_col;
	t_light_list	*light_node;

	dif_col = (t_color){0, 0, 0};
	spec_col = (t_color){0, 0, 0};
	light_node = rtv->lights;
	while (light_node)
	{
		ft_refracted_ray(&(rtv->cam), &(light_node->light), normal);
		if (ft_check_shadow(rtv, light_node->light, normal, &color))
		{
			dif_col = ft_add_colors(dif_col,
				ft_diffuse(light_node->light, normal, color));
			spec_col = ft_add_colors(spec_col,
				ft_specular(light_node->light, normal));
		}
		light_node = light_node->next;
	}
	color = ft_add_colors(dif_col, ft_add_colors(spec_col,
		ft_scale_colors(color, AMBIANT)));
	return (color);
}
