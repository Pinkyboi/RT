/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   slaved.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merras <merras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 17:17:02 by merras            #+#    #+#             */
/*   Updated: 2020/03/03 17:28:04 by merras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "slaved.h"

int	accept_master(t_slaved *slaved, t_sa *master_address)
{
	socklen_t	address_length;
	int			err;

	err = 0;
	if ((slaved->sockets[1] = accept(slaved->sockets[0],
		(struct sockaddr *)master_address, &address_length)) == -1)
		return (1);
	LOG_INFO("accepting a master connection.");
	err = ERROR_WRAPPER(pthread_create(&(slaved->tid[0]), NULL,
		receive_packets, slaved));
	err = ERROR_WRAPPER(pthread_create(&(slaved->tid[1]), NULL,
		handle_packets, slaved));
	if (err)
		return (1);
	if (!pthread_join(slaved->tid[0], NULL))
	{
		LOG_INFO("routine joined, cleaning up.");
		pthread_cancel(slaved->tid[1]);
		pthread_join(slaved->tid[0], NULL);
		slaved_cleanup(slaved);
	}
	LOG_INFO("disconnecting a master.");
	close(slaved->sockets[0]);
	close(slaved->sockets[1]);
	return (0);
}

int	connect_master(t_slaved *slaved)
{
	struct sockaddr_in slave_address;
	struct sockaddr_in master_address;

	if ((slaved->sockets[0] = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		return (1);
	ft_bzero(&slave_address, sizeof(slave_address));
	ft_bzero(&master_address, sizeof(master_address));
	slave_address.sin_family = AF_INET;
	slave_address.sin_addr.s_addr = htonl(INADDR_ANY);
	slave_address.sin_port = htons(PORT);
	if (bind(slaved->sockets[0], (struct sockaddr *)&slave_address,
			sizeof(slave_address)) ||
		listen(CAST(slaved, t_slaved *)->sockets[0], 1))
		return (2);
	if (accept_master(slaved, &master_address))
		return (3);
	return (0);
}

int	main(void)
{
	t_slaved	slaved;
	int			err;

	pthread_mutex_init(&slaved.packets_queue_mx, NULL);
	slaved.packets_queue = t_dstruct_list_init();
	ft_bzero(&slaved.program, sizeof(t_program));
	slaved.program.pid = -2;
	if ((err = connect_master(&slaved)))
		LOG_ERROR("error connecting master, %d.", err);
	return (err);
}
