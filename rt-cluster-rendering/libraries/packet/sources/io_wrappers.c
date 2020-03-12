/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_wrappers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merras <merras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/21 17:55:08 by merras            #+#    #+#             */
/*   Updated: 2020/03/02 14:00:55 by merras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "packet_utilities.h"

int	read_(int fd, char *buffer, size_t size)
{
	int remaining;
	int ret;

	remaining = size;
	while (remaining)
	{
		if ((ret = read(fd, buffer + size - remaining, remaining)) < 1)
			return (0);
		LOG_DEBUG("reading a slice of %d bytes.", ret);
		remaining -= ret;
	}
	return (size);
}

int	write_(int fd, char *buffer, size_t size)
{
	int remaining;
	int ret;

	remaining = size;
	while (remaining)
	{
		if ((ret = write(fd, buffer + size - remaining, remaining)) < 1)
			return (0);
		LOG_DEBUG("writing a slice of %d bytes.", ret);
		remaining -= ret;
	}
	return (size);
}
