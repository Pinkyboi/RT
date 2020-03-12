/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_marching.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiri <abiri@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/04 11:39:32 by abiri             #+#    #+#             */
/*   Updated: 2020/03/04 11:40:51 by abiri            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

double	trace(t_vector from, t_vector direction,
	t_fractal *fractal, int *status)
{
	double		total_distance;
	int			steps;
	double		distance;
	t_vector	p;

	distance = INFINITY;
	total_distance = 0.0;
	*status = 0;
	steps = -1;
	while (++steps < fractal->max_steps)
	{
		p = ft_add_vector(from, ft_scale_vector(direction, total_distance));
		distance = ft_mandelbrot_distance_estimator(p, status, *fractal);
		total_distance += distance;
		if (distance < MIN_D)
			break ;
	}
	return (total_distance);
}
