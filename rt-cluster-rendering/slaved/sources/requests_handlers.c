/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requests_handlers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merras <merras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/01 22:26:17 by merras            #+#    #+#             */
/*   Updated: 2020/03/03 01:30:32 by merras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "slaved.h"

int			write_pipe_slaved_program(t_packet *request, t_program *program,
				int *stdin_pipe, int *stdout_pipe)
{
	int fd;

	if ((fd = open(SLAVED_PROGRAM_NAME, O_CREAT | O_WRONLY, 0777)) == -1)
		return (1);
	if (write_(fd, request->data, request->size) != (int)request->size)
		return (1);
	if (pipe(stdin_pipe) == -1 || pipe(stdout_pipe) == -1)
		return (1);
	program->w_stdin = stdin_pipe[1];
	program->r_stdout = stdout_pipe[0];
	return (0);
}

void		piping_slaved_program(t_slaved *slaved, int *stdin_pipe,
				int *stdout_pipe)
{
	dup2(stdin_pipe[0], 0);
	dup2(stdout_pipe[1], 1);
	close(stdin_pipe[0]);
	close(stdout_pipe[1]);
	close(slaved->sockets[0]);
	close(slaved->sockets[1]);
}

t_packet	execute_req_init(t_packet *request, t_slaved *slaved)
{
	int stdin_pipe[2];
	int stdout_pipe[2];

	if (write_pipe_slaved_program(request, &slaved->program, stdin_pipe,
		stdout_pipe))
	{
		LOG_ERROR("writing slaved program and piping failed.");
		return (FLAG_RESPONSE_PACKET(TYPE_T_RESPONSE_FAILURE));
	}
	if (!(slaved->program.pid = fork()))
	{
		piping_slaved_program(slaved, stdin_pipe, stdout_pipe);
		if (execv(SLAVED_PROGRAM_NAME, NULL))
			LOG_WARN("\texecute_req_init(): execv() failed.");
		exit(0);
	}
	else if (slaved->program.pid == -1)
	{
		LOG_ERROR("forking for slaved program failed.");
		return (FLAG_RESPONSE_PACKET(TYPE_T_RESPONSE_FAILURE));
	}
	close(stdin_pipe[0]);
	close(stdout_pipe[1]);
	return (FLAG_RESPONSE_PACKET(TYPE_T_RESPONSE_SUCCESS));
}

t_packet	execute_req_scene(t_packet *request, t_program *program)
{
	int err;
	int error;

	err = 0;
	LOG_DEBUG("executing a TYPE_T_REQUEST_SCENE.");
	err = ERROR_WRAPPER(write(program->w_stdin, request->data, request->size)
		!= request->size);
	if (err)
		LOG_ERROR("error writing request data.");
	err = ERROR_WRAPPER(write(program->w_stdin, "!\n", 2) != 2);
	if (err)
		LOG_ERROR("error writing scene data delimiter !.");
	err = ERROR_WRAPPER(read(program->r_stdout, &error, 4) != 4);
	if (err)
		LOG_ERROR("error reading scene parsing status.");
	else
		LOG_DEBUG("scene parsing status: %s", error == -2 ? "error" : "fine");
	if (err || error == -2)
		return (FLAG_RESPONSE_PACKET(TYPE_T_RESPONSE_FAILURE));
	return (FLAG_RESPONSE_PACKET(TYPE_T_RESPONSE_SUCCESS));
}

t_packet	execute_req_computation(t_packet *request, t_program *program)
{
	t_packet	response;
	int			err;

	err = 0;
	LOG_DEBUG("executing a TYPE_T_REQUEST_COMPUTATION.");
	err = ERROR_WRAPPER(write(program->w_stdin, request->data, request->size)
		!= request->size);
	if (err)
		LOG_ERROR("error writing input data.");
	err = ERROR_WRAPPER((read(program->r_stdout, &response.size,
		sizeof(response.size))) != sizeof(response.size));
	if (err)
		LOG_WARN("error reading computation output SIZE.");
	if (!err && !(response.data = malloc(response.size)))
		return (FLAG_RESPONSE_PACKET(TYPE_T_RESPONSE_FAILURE));
	err = ERROR_WRAPPER(read_(program->r_stdout, response.data, response.size)
		!= (int)response.size);
	if (!err)
	{
		LOG_INFO("successful computation request execution: %d.",
			response.size);
		response.type = TYPE_T_RESPONSE_SUCCESS;
		return (response);
	}
	return (FLAG_RESPONSE_PACKET(TYPE_T_RESPONSE_FAILURE));
}
