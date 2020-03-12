/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packet_utilities.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merras <merras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 23:05:56 by merras            #+#    #+#             */
/*   Updated: 2020/02/29 23:08:30 by merras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PACKET_UTILITIES_H
# define PACKET_UTILITIES_H

# include <stdint.h>
# include <unistd.h>
# include <stdlib.h>

# include "log.h"

typedef struct	s_packet
{
	void		*id;
	uint8_t		type;
	uint32_t	size;
	void		*data;
}				t_packet;

t_packet		*create_packet(t_packet value, void *id);
void			destroy_packet(void *packet);

/*
** write_packet() and read_packet() return 0 upon successful execution,
** 1 otherwise.
*/

int				write_packet(int fd, t_packet packet);
int				read_packet(int fd, t_packet *packet);

int				read_(int fd, char *buffer, size_t size);
int				write_(int fd, char *buffer, size_t size);

/*
** RESPONSE_PACKET is meant to be considered as flag response when
** the size is 0.
*/

# define FLAG_RESPONSE_PACKET(flag) (t_packet) { NULL, flag, 0, NULL }
# define ERROR_WRAPPER(cond) err ? err : cond

#endif
