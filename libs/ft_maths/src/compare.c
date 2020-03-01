/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compare.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/05 16:21:20 by abenaiss          #+#    #+#             */
/*   Updated: 2020/01/01 02:15:50 by abenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_maths.h"

int		ft_vector_null(t_vector v)
{
	return ((v.x == 0.0 && v.y == 0.0
	&& v.z == 0.0) ? 1 : 0);
}

int		ft_vector_limit(t_vector v1, t_vector v2)
{
	return ((v1.x > v2.x || v1.y > v2.y
	|| v1.z > v2.z) ? 1 : 0);
}

double	ft_vector_size(t_vector u)
{
	return (sqrt(u.x * u.x + u.y * u.y + u.z * u.z));
}
