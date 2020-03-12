/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   slaved.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merras <merras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 17:17:15 by merras            #+#    #+#             */
/*   Updated: 2020/03/03 00:49:24 by merras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SLAVED_H
# define SLAVED_H

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <pthread.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/types.h>
# include <sys/time.h>
# include <signal.h>

# include "cdata_structures.h"
# include "log.h"
# include "centropy.h"
# include "packet_utilities.h"

# define SLAVED_PROGRAM_NAME "slaved_program"
# define PORT 5000
# define ERROR_WRAPPER(cond) err ? err : cond

typedef struct sockaddr_in	t_sa;

typedef struct	s_program
{
	pid_t	pid;
	int		w_stdin;
	int		r_stdout;
}				t_program;

typedef struct	s_slaved
{
	int				sockets[2];
	pthread_mutex_t	packets_queue_mx;
	t_dstruct_list	packets_queue;
	pthread_t		tid[2];
	t_program		program;
}				t_slaved;

t_packet		requests_dispatcher(t_packet *request, t_slaved *slaved);
t_packet		execute_req_init(t_packet *request, t_slaved *slaved);
t_packet		execute_req_computation(t_packet *request, t_program *program);
t_packet		execute_req_scene(t_packet *request, t_program *program);
void			slaved_cleanup(t_slaved *slaved);

# define TYPE_T_REQUEST_INIT 0
# define TYPE_T_REQUEST_COMPUTATION 1
# define TYPE_T_REQUEST_SCENE 2

# define TYPE_T_RESPONSE_SUCCESS 0
# define TYPE_T_RESPONSE_FAILURE 1

void			*receive_packets(void *slaved);
void			*handle_packets(void *slaved);

#endif
