/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   slaved_cleanup.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merras <merras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/01 20:52:38 by merras            #+#    #+#             */
/*   Updated: 2020/03/03 15:28:58 by merras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "slaved.h"

void	slaved_cleanup(t_slaved *slaved)
{
	if (slaved->program.pid != -2)
		kill(slaved->program.pid, 3);
	close(slaved->program.r_stdout);
	close(slaved->program.w_stdin);
	list_delete(&slaved->packets_queue, destroy_packet);
}
