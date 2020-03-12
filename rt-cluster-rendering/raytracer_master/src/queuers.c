/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queuers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merras <merras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/02 22:30:08 by merras            #+#    #+#             */
/*   Updated: 2020/03/04 20:55:24 by merras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "master_raytracer.h"

int		ft_send_scene(t_cluster *cluster, char *filename)
{
	t_packet	packet;

	if (!(packet.data = read_file(open(filename, O_RDONLY), &packet.size)))
		return (0);
	packet.type = TYPE_T_REQUEST_SCENE;
	queue_task(cluster, create_packet(packet, NULL));
	return (1);
}

int		ft_send_tasks(t_cluster *cluster)
{
	int			index;
	int			*height_interval;
	t_packet	packet;

	index = 0;
	while (index < (int)cluster->size)
	{
		if (!(height_interval = malloc(sizeof(int) * 2)))
			return (0);
		height_interval[0] = (HEIGHT / cluster->size) * index;
		height_interval[1] = height_interval[0] + (HEIGHT / cluster->size);
		packet.data = height_interval;
		packet.size = sizeof(int) * 2;
		packet.type = TYPE_T_REQUEST_COMPUTATION;
		queue_task(cluster, create_packet(packet, NULL));
		index++;
	}
	return (1);
}

void	destroy_task(void *task)
{
	destroy_packet(CAST(task, t_task *)->request);
	destroy_packet(CAST(task, t_task *)->response);
}
