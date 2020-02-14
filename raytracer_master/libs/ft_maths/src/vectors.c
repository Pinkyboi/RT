/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vectors.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/22 15:23:08 by azarzor           #+#    #+#             */
/*   Updated: 2020/01/02 15:08:54 by abenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_maths.h"

t_vector	ft_new_vector(double x, double y, double z)
{
	t_vector	res;

	res.x = x;
	res.y = y;
	res.z = z;
	return (res);
}

t_vector	ft_add_vector(t_vector v1, t_vector v2)
{
	t_vector	res;

	res.x = v1.x + v2.x;
	res.y = v1.y + v2.y;
	res.z = v1.z + v2.z;
	return (res);
}

t_vector	ft_sub_vector(t_vector v1, t_vector v2)
{
	t_vector	res;

	res.x = v1.x - v2.x;
	res.y = v1.y - v2.y;
	res.z = v1.z - v2.z;
	return (res);
}

t_vector	ft_mult_vector(t_vector v1, t_vector v2)
{
	t_vector	res;

	res.x = v1.x * v2.x;
	res.y = v1.y * v2.y;
	res.z = v1.z * v2.z;
	return (res);
}

t_vector	ft_div_vector(t_vector v1, t_vector v2)
{
	t_vector	res;

	res.x = (double)v1.x / v2.x;
	res.y = (double)v1.y / v2.y;
	res.z = (double)v1.z / v2.z;
	return (res);
}
