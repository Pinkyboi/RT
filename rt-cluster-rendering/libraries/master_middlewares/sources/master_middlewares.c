/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   master_middlewares.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merras <merras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/02 00:56:49 by merras            #+#    #+#             */
/*   Updated: 2020/03/03 15:45:23 by merras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "master_middlewares.h"

int	init_cluster(char *configuration_file, t_cluster *cluster)
{
	int	err;

	err = 0;
	LOG_INFO("initiating cluster.");
	ft_bzero(cluster, sizeof(t_cluster));
	err = ERROR_WRAPPER(get_configuration(configuration_file, cluster) != 0);
	err = ERROR_WRAPPER(connect_slaves(cluster) != 0);
	err = ERROR_WRAPPER(init_computation(cluster));
	err = ERROR_WRAPPER(init_slaves(cluster) != 0);
	err = ERROR_WRAPPER(pthread_create(&cluster->loadbalancer_routine, NULL,
		cluster_loadbalancer, cluster) != 0);
	return (err);
}
