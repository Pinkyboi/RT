/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compare.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/05 16:21:20 by abenaiss          #+#    #+#             */
/*   Updated: 2019/12/20 17:08:14 by abenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_maths.h"

int     ft_vector_null(t_vector v)
{
    return((v.x == (double)0 && v.y == (double)0
        && v.z == (double)0) ? 1 : 0);
}

int     ft_vector_limit(t_vector v1, t_vector v2)
{
    return((v1.x > v2.x || v1.y > v2.y
        || v1.z > v2.z)? 1 : 0);
}