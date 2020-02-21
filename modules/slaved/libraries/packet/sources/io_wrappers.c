/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_wrappers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merras <merras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/21 17:55:08 by merras            #+#    #+#             */
/*   Updated: 2020/02/21 19:04:24 by merras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "packet_utilities.h"

int	_read(int fd, char *buffer, size_t size)
{
	int remaining;
	int	ret;

	remaining = size;
	while (remaining)
	{
		if ((ret = read(fd, buffer + size - remaining, remaining)) < 1)
			return (ret);
		log_debug("reading a slice of %d bytes.", ret);
		remaining -= ret;
	}
	return (size);
}

int	_write(int fd, char *buffer, size_t size)
{
	int remaining;
	int	ret;

	remaining = size;
	while (remaining)
	{
		if ((ret = write(fd, buffer + size - remaining, remaining)) < 1)
			return (ret);
		log_debug("writing a slice of %d bytes.", ret);
		remaining -= ret;
	}
	return (size);
}