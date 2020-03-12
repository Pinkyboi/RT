/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merras <merras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/02 06:21:46 by merras            #+#    #+#             */
/*   Updated: 2020/03/04 15:42:15 by merras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "master_middlewares.h"

void	cancel_routines(t_cluster *cluster, void *caller)
{
	int i;

	i = -1;
	while (++i < (int)cluster->size)
	{
		if (&cluster->nodes[i] == caller)
			continue;
		else
		{
			pthread_cancel(cluster->nodes[i].tid);
			pthread_join(cluster->nodes[i].tid, NULL);
		}
		close(cluster->nodes[i].socket);
		free(&cluster->nodes[i].ip);
		list_delete(&cluster->nodes[i].tasks_queue, destroy_packet);
	}
}

void	delete_nodes(t_slave *nodes, int size)
{
	while (--size)
		free(nodes[size].ip);
	free(nodes);
}
