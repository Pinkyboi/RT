
/*
** basically, a cleanup() function should be provided,
** and a requests_dispatcher() based on how many requests type you have.
*/

#include "slaved.h"

void    *init_computation()
{
    return (malloc(sizeof(t_program)));
}

void    cleanup(void *computation)
{
	int cleanup_proc;

    kill(SIGKILL, CAST(computation, t_program *)->pid);
    close(CAST(computation, t_program *)->w_stdin);
    close(CAST(computation, t_program *)->r_stdout);
    free(computation);
	if (!(cleanup_proc = fork()))
	{
		execv("/bin/rm", (char *[4]){"rm", "-rf", TEXTURES_DIR, NULL});
		exit(0);
	}
}

t_packet  requests_dispatcher(t_packet *request, void *program, int cfd[2])
{
    if (request->type == TYPE_T_REQUEST_INIT)
        return (execute_req_init(request, program, cfd));
    if (request->type == TYPE_T_REQUEST_COMPUTATION)
        return (execute_req_computation(request, program));
	if (request->type == TYPE_T_REQUEST_SCENE)
		return(execute_req_scene(request, program));
	if (request->type == TYPE_T_REQUEST_COMPUTATION)
		return(execute_req_computation(request, program));
	if (request->type == TYPE_T_REQUEST_FINISH)
        return (FLAG_RESPONSE_PACKET(TYPE_T_RESPONSE_SUCCESS));
    return ((t_packet){NULL, 0, 0, NULL});
}