/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requests_dispatcher.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merras <merras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/01 20:57:25 by merras            #+#    #+#             */
/*   Updated: 2020/03/03 01:30:31 by merras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "slaved.h"

t_packet	requests_dispatcher(t_packet *request, t_slaved *slaved)
{
	if (request->type == TYPE_T_REQUEST_INIT)
		return (execute_req_init(request, slaved));
	if (request->type == TYPE_T_REQUEST_SCENE)
		return (execute_req_scene(request, &slaved->program));
	if (request->type == TYPE_T_REQUEST_COMPUTATION)
		return (execute_req_computation(request, &slaved->program));
	return ((t_packet){NULL, 0, 0, NULL});
}
