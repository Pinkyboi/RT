/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packet.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merras <merras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/22 14:11:51 by merras            #+#    #+#             */
/*   Updated: 2020/03/02 21:25:04 by merras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "packet_utilities.h"

int			read_packet(int fd, t_packet *packet)
{
	int err;

	err = 0;
	err = ERROR_WRAPPER((read(fd, &packet->type, sizeof(packet->type))) !=
		(int)sizeof(packet->type));
	if (!err)
		LOG_INFO("\treading packet type: %d.", packet->type);
	err = ERROR_WRAPPER(read(fd, &packet->size, sizeof(packet->size)) !=
		(int)sizeof(packet->size));
	if (!err)
		LOG_INFO("\treading packet size: %d.", packet->size);
	if (!err && !(packet->data = malloc(sizeof(char) * packet->size)))
		err = 1;
	if (!err)
		LOG_INFO("allocating for packet data.");
	err = ERROR_WRAPPER(read_(fd, packet->data, packet->size) !=
		(int)packet->size);
	if (!err)
		LOG_INFO("reading packet data.");
	return (err);
}

int			write_packet(int fd, t_packet packet)
{
	int err;

	err = 0;
	err = ERROR_WRAPPER(write(fd, &packet.type, sizeof(uint8_t)) == -1);
	if (!err)
		LOG_INFO("writing packet type.");
	if (!err)
	{
		err = ERROR_WRAPPER(write(fd, &packet.size, sizeof(uint32_t)) == -1);
		if (!err)
			LOG_INFO("writing packet size: %d.", packet.size);
		err = ERROR_WRAPPER(write_(fd, packet.data, packet.size) !=
			(int)packet.size);
		if (!err)
			LOG_INFO("writing packet data.");
		else
			LOG_WARN("error writing packet data.");
	}
	return (err);
}

t_packet	*create_packet(t_packet value, void *id)
{
	t_packet *packet;

	if (!(packet = malloc(sizeof(t_packet))))
		return (NULL);
	packet->id = id;
	packet->type = value.type;
	packet->size = value.size;
	packet->data = value.data;
	return (packet);
}

void		destroy_packet(void *packet)
{
	free(((t_packet *)packet)->data);
	free(packet);
}
