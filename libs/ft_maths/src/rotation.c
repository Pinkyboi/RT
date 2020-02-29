/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotation.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/05 16:21:20 by abenaiss          #+#    #+#             */
/*   Updated: 2019/12/05 16:42:21 by abenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_maths.h"

t_vector	ft_rotate_vectorx(t_vector a, double angle)
{
	t_vector	result;

	result.x = a.x;
	result.z = a.z * cos(angle) - a.y * sin(angle);
	result.y = a.z * sin(angle) + a.y * cos(angle);
	return (result);
}

t_vector	ft_rotate_vectory(t_vector a, double angle)
{
	t_vector	result;

	result.x = a.x * cos(angle) - a.z * sin(angle);
	result.y = a.y;
	result.z = a.x * sin(angle) + a.z * cos(angle);
	return (result);
}

t_vector	ft_rotate_vectorz(t_vector a, double angle)
{
	t_vector	result;

	result.x = a.x * cos(angle) - a.y * sin(angle);
	result.y = a.x * sin(angle) + a.y * cos(angle);
	result.z = a.z;
	return (result);
}

t_vector	ft_rotate_vector(t_vector a, t_vector angles)
{
	t_vector	result;

	result = ft_rotate_vectorx(a, FT_RAD(angles.x));
	result = ft_rotate_vectory(result, FT_RAD(angles.y));
	result = ft_rotate_vectorz(result, FT_RAD(angles.z));
	return (ft_normalise_vector(result));
}
