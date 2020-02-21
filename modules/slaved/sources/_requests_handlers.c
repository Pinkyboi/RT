#include "slaved.h"

int     write_program(int fd, void *data, int size)
{
    int ret;
    int remaining;

    remaining = size;
    while (1)
    {
        if (!remaining)
            break ;
        if ((ret = write(fd, data, remaining)) == -1)
            return (1);
        remaining -= ret;
    }
    return (0);
}

#include <string.h>
#include <errno.h>

t_packet    execute_req_init(t_packet *request, t_program *program, int cfd[2])
{
    int fd;
    int stdin_pipe[2];
    int stdout_pipe[2];

    if ((fd = open(SLAVED_PROGRAM_NAME, O_CREAT | O_WRONLY, 0777)) == -1)
	{
		log_warn("\texecute_req_init(): error opening a executable file.");
        return (FLAG_RESPONSE_PACKET(TYPE_T_RESPONSE_FAILURE));
	}
    if (mkdir(TEXTURES_DIR, 0644) == -1)
	{
		log_warn("\texecute_req_init(): error creating textures directory.");
        return (FLAG_RESPONSE_PACKET(TYPE_T_RESPONSE_FAILURE));
	}
    if (pipe(stdin_pipe) == -1 || pipe(stdout_pipe) == -1)
	{
		log_warn("\texecute_req_init(): error creating pipe.");
        return (FLAG_RESPONSE_PACKET(TYPE_T_RESPONSE_FAILURE));
	}
    program->w_stdin = stdin_pipe[1];
    program->r_stdout = stdout_pipe[0];
    if (write_program(fd, request->data, request->size))
	{
		log_warn("\texecute_req_init(): error writing program text.");
        return (FLAG_RESPONSE_PACKET(TYPE_T_RESPONSE_FAILURE));
	}
    if (!(program->pid = fork()))
    {
        dup2(stdin_pipe[0], 0);
        close(stdin_pipe[0]);
        dup2(stdout_pipe[1], 1);
        close(stdout_pipe[1]);
		close(cfd[0]);
		close(cfd[1]);
        if (execv(SLAVED_PROGRAM_NAME, (char *[2]){SLAVED_PROGRAM_NAME, NULL}) == -1)
		{
			log_warn("\texecute_req_init(): execve of slaved_program failed(): %s.", strerror(errno));
            exit (0);
		}
    }
    else if (program->pid == -1)
	{
		log_warn("\texecute_req_init(): forking slaved_program failed.");
        return (FLAG_RESPONSE_PACKET(TYPE_T_RESPONSE_FAILURE));
	}
    close(stdin_pipe[0]);
    close(stdout_pipe[1]);
	log_debug("request execution logs:");
	log_debug("\tslaved_program pid: %d", program->pid);
	log_debug("\tslaved_program io: %d %d", program->r_stdout, program->w_stdin);
    return (FLAG_RESPONSE_PACKET(TYPE_T_RESPONSE_SUCCESS));
}

t_packet    execute_req_scene(t_packet *request, t_program *program)
{
    int err;
	int error;

    err = 0;
	log_debug("executing a TYPE_T_REQUEST_SCENE.");
    err = ERROR_WRAPPER(write(program->w_stdin, request->data, request->size) != request->size);
	if (err)
		log_warn("error writing request data.");
    err = ERROR_WRAPPER(write(program->w_stdin, "!\n", 2) != 2);
	if (err)
		log_warn("error writing scene data delimiter !.");
	err = ERROR_WRAPPER(read(program->r_stdout, &error, 4) != 4);
	if (err)
		log_warn("error reading scene parsing status.");
	else
		log_debug("scene parsing status: %s", error == -2 ? "error" : "fine");
    if (err || error == -2)
        return (FLAG_RESPONSE_PACKET(TYPE_T_RESPONSE_FAILURE));
    return (FLAG_RESPONSE_PACKET(TYPE_T_RESPONSE_SUCCESS));
}

t_packet    execute_req_texture(t_packet *request, t_program *program)
{
    int ret;
    int     texture_fd;
    char    texture_filename[TEXTURES_FILENAME_SIZE + 2];
    char    *texture_path;

    ft_bzero(texture_filename, TEXTURES_FILENAME_SIZE + 2);
    texture_filename[0] = '/';

    ret = 0;
    ft_strncpy(request->data, texture_filename + 1, TEXTURES_FILENAME_SIZE);
    texture_path = ft_strjoin(TEXTURES_DIR, texture_filename);
    if ((texture_fd = open(texture_path, 0644)) == -1)
        return (FLAG_RESPONSE_PACKET(TYPE_T_RESPONSE_FAILURE));
    ret = write(texture_fd, request->data + TEXTURES_FILENAME_SIZE,
        request->size - TEXTURES_FILENAME_SIZE);
    if (ret)
        return (FLAG_RESPONSE_PACKET(TYPE_T_RESPONSE_FAILURE));
    return (FLAG_RESPONSE_PACKET(TYPE_T_RESPONSE_SUCCESS));
}

t_packet    execute_req_computation(t_packet *request, t_program *program)
{
    t_packet    response;
    int         err;
	int			ret;

    err = 0;
	log_debug("executing a TYPE_T_REQUEST_COMPUTATION.");
    err = ERROR_WRAPPER(write(program->w_stdin, request->data, request->size) != request->size);
	if (err)
		log_warn("error writing input data.");
    err = ERROR_WRAPPER((ret = read(program->r_stdout, &response.size, sizeof(response.size))) != sizeof(response.size));
	if (err)
		log_warn("error reading computation output SIZE, read return: %d.", ret);
	else
		log_debug("read the output size: %d.", response.size);
    if (!err && !(response.data = malloc(response.size)))
        return (FLAG_RESPONSE_PACKET(TYPE_T_RESPONSE_FAILURE));

	int	i = response.size;
	int rr;
	while (i)
	{
		if ((rr = read(program->r_stdout, response.data + response.size - i, i)) < 1)
		{
			err = 1;
			break ;
		}
		i -= rr;
		log_debug("reading a segment from output data, size: %d, remaining: %d.", rr, i);
	}
	if (!err)
	{
		log_info("successful computation request execution: %d.", response.size);
		response.type = TYPE_T_RESPONSE_SUCCESS;
    	return (response);
	}
    return (FLAG_RESPONSE_PACKET(TYPE_T_RESPONSE_FAILURE));
}