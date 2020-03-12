/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connect_slaves.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merras <merras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/02 06:12:35 by merras            #+#    #+#             */
/*   Updated: 2020/03/04 17:26:17 by merras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "master_middlewares.h"

char	*read_file(int fd, uint32_t *size)
{
	char		buffer[BUFFER_SIZE];
	struct stat	file_stat;
	int			offset;
	int			read_return;
	char		*content;

	if (fd == -1 || fstat(fd, &file_stat) == -1)
		return (NULL);
	*size = file_stat.st_size;
	content = malloc(file_stat.st_size);
	offset = 0;
	while ((read_return = read(fd, buffer, BUFFER_SIZE - 1)) > 0)
	{
		ft_memcpy(content + offset, buffer, read_return);
		offset += read_return;
	}
	return (content);
}

int		init_slaves(t_cluster *cluster)
{
	t_packet	request;
	int			err;

	LOG_INFO("initiating slaves.");
	err = 0;
	request.type = TYPE_T_REQUEST_INIT;
	if (!(request.data = read_file(cluster->program, &request.size)))
		err = 1;
	if (!err)
		queue_task(cluster, create_packet(request, NULL));
	return (err);
}

int		connect_slave(int i, t_cluster *cluster)
{
	struct sockaddr_in	slave;
	int					err;

	err = 0;
	if ((cluster->nodes[i].socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		err = 1;
	slave.sin_family = AF_INET;
	slave.sin_addr.s_addr = inet_addr(cluster->nodes[i].ip);
	slave.sin_port = htons(PORT);
	err = ERROR_WRAPPER(connect(cluster->nodes[i].socket,
		(struct sockaddr *)&slave, sizeof(slave)) != 0);
	if (err)
	{
		i = -1;
		while (++i < (int)cluster->size)
			pthread_cancel(cluster->nodes[i].tid);
		delete_nodes(cluster->nodes, cluster->size);
		return (1);
	}
	err = ERROR_WRAPPER(pthread_create(&cluster->nodes[i].tid,
		NULL, slave_routine, &(cluster->nodes[i])));
	return (err);
}

int		connect_slaves(t_cluster *cluster)
{
	int					i;

	i = -1;
	while (++i < (int)cluster->size)
	{
		if (connect_slave(i, cluster))
			return (1);
	}
	return (0);
}
