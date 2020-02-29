/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiri <kerneloverseer@pm.me>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/06 15:17:26 by abiri             #+#    #+#             */
/*   Updated: 2019/12/06 15:20:31 by abiri            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static void		ft_atof_integer(char *string, double *number, size_t *index)
{
	while (string[*index])
	{
		if (ft_isdigit(string[*index]))
			*number = *number * 10 + string[*index] - '0';
		else
			return ;
		(*index)++;
	}
}

static void		ft_atof_floating(char *string, double *number, size_t *index)
{
	double	mult;

	mult = 0.1;
	while (string[*index])
	{
		if (ft_isdigit(string[*index]))
		{
			*number += mult * (string[*index] - '0');
			mult /= 10;
		}
		else
			return ;
		(*index)++;
	}
}

double			ft_atof(char *string, int *size)
{
	size_t		index;
	double		number;
	int			sign;

	number = 0;
	sign = 1;
	index = ft_escape_whitespaces(string);
	if (string[index] == '-' || string[index] == '+')
		sign = (string[index++] == '-') ? -1 : 1;
	ft_atof_integer(string, &number, &index);
	if (string[index] == '.')
	{
		index++;
		ft_atof_floating(string, &number, &index);
	}
	*size = index + 1;
	return (number * sign);
}
