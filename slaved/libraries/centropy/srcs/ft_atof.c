/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merras <merras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/29 19:18:28 by merras            #+#    #+#             */
/*   Updated: 2020/01/26 16:06:31 by merras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "centropy.h"

static void	ft_atof_sign(char *s, int *flags, int *i)
{
	while (s[++(*i)] == ' ' || s[*i] == '\t' || s[*i] == '\n')
		continue ;
	if (s[*i] == '+' || s[*i] == '-')
	{
		flags[SIGN] = (s[*i] == '-') * -1;
		(*i)++;
	}
}

double		ft_atof(char *s, int precision)
{
	double	fpoint;
	int		i;
	int		flags[4];
	double	tens;

	ft_bzero(flags, 4 * sizeof(int));
	fpoint = 0;
	i = -1;
	tens = 10;
	ft_atof_sign(s, flags, &i);
	while ((s[i] >= '0' && s[i] <= '9') ||
			(s[i] == '.' && !flags[DOT] && flags[FIRST_ITERATION]))
	{
		if (s[i] == '.' && (flags[DOT] = 1) && i++)
			continue ;
		flags[PRECISION] += flags[DOT];
		fpoint = flags[DOT] ? fpoint + (s[i] - '0') / tens :
			fpoint * 10 + (s[i] - '0');
		tens = flags[DOT] ? tens * 10 : tens;
		flags[FIRST_ITERATION] = 1;
		if (flags[PRECISION] > precision)
			return (fpoint);
		i++;
	}
	return ((flags[SIGN] ? -1 : 1) * fpoint);
}
