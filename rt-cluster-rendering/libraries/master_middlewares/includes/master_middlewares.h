/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   master_middlewares.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merras <merras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/02 00:56:37 by merras            #+#    #+#             */
/*   Updated: 2020/03/03 00:51:19 by merras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MASTER_MIDDLEWARES_H
# define MASTER_MIDDLEWARES_H

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <pthread.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/types.h>
# include <sys/time.h>

# include "centropy.h"
# include "cdata_structures.h"
# include "packet_utilities.h"
# include "log.h"

# define PORT 5000
# define ERROR_WRAPPER(cond) err ? err : cond
# define BUFFER_SIZE 69

typedef struct	s_slave
{
	char			*ip;
	int				socket;
	void			*cluster;
	pthread_mutex_t	mutex;
	t_dstruct_list	tasks_queue;
	pthread_t		tid;
}				t_slave;

typedef struct	s_computation
{
	pthread_mutex_t	tasks_queue_mutex;
	pthread_mutex_t	done_queue_mutex;
	t_dstruct_list	tasks_queue;
	t_dstruct_list	done_queue;
	int				size;
}				t_computation;

typedef struct	s_cluster
{
	t_slave			*nodes;
	int				offset;
	size_t			size;
	t_computation	computation;
	int				program;
	int				error;
	pthread_t		loadbalancer_routine;
}				t_cluster;

typedef struct	s_task
{
	t_packet	*request;
	t_packet	*response;
}				t_task;

int				init_cluster(char *configuration_file, t_cluster *cluster);
int				connect_slaves(t_cluster *cluster);
int				init_slaves(t_cluster *cluster);

void			*slave_routine(void *slave);

void			*cluster_loadbalancer(void *cluster);
void			delete_nodes(t_slave *nodes, int size);

void			**fetch_computation_blob(t_cluster *cluster,
					int (*compare)(void *, void *));
int				init_computation(t_cluster *cluster);
void			queue_task(t_cluster *cluster, t_packet *request);

void			cancel_routines(t_cluster *cluster, void *caller);

# define TYPE_T_REQUEST_INIT 0
# define TYPE_T_REQUEST_COMPUTATION 1
# define TYPE_T_REQUEST_SCENE 2

# define TYPE_T_RESPONSE_SUCCESS 0
# define TYPE_T_RESPONSE_FAILURE 1

/*
** CONFIGURATION SHIT
*/

# define BYTE_BLOCK_LOCK 0
# define BYTE_BLOCK 1
# define BYTE_BLOCK_OFFSET 2
# define DOTS 3

int				get_configuration(char *configuration_file, t_cluster *cluster);
char			*read_file(int fd, uint32_t *size);

#endif
