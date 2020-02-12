/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vectors2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/22 15:50:11 by azarzor           #+#    #+#             */
/*   Updated: 2019/12/05 16:42:18 by abenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_maths.h"

t_vector	ft_normalise_vector(t_vector u)
{
	double		size;
	t_vector	result;

	size = ft_vector_size(u);
	result.x = u.x / size;
	result.y = u.y / size;
	result.z = u.z / size;
	return (result);
}

t_vector	ft_scale_vector(t_vector u, double scale)
{
	t_vector	result;

	result.x = u.x * scale;
	result.y = u.y * scale;
	result.z = u.z * scale;
	return (result);
}

double		ft_dot_vector(t_vector v1, t_vector v2)
{
	t_vector	v;
	double		r;

	v.x = v1.x * v2.x;
	v.y = v1.y * v2.y;
	v.z = v1.z * v2.z;
	r = v.x + v.y + v.z;
	return (r);
}

t_vector	ft_cross_product(t_vector a, t_vector b)
{
	t_vector	result;

	result.x = ((a.y * b.z) - (a.z * b.y));
	result.y = ((a.z * b.x) - (a.x * b.z));
	result.z = ((a.x * b.y) - (a.y * b.x));
	return (result);
}
