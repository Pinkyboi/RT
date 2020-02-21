#ifndef _REQUESTS_HANDLERS_H
# define _REQUESTS_HANDLERS_H

typedef struct  s_program
{
    pid_t       pid;
    int         w_stdin;
    int         r_stdout;
    int         
}               t_program;

void    *init_computation();
void    cleanup(void *computation);
t_packet  requests_dispatcher(t_packet *request, void *computation);


t_packet  execute_req_init(t_packet *request, t_program *program);
t_packet  execute_req_computation(t_packet *request, t_program *program);


/*
** SLAVED CONVENTIONS
*/

# define FINISH_PACKET_TYPE 2

# define TYPE_T_REQUEST_INIT 0
# define TYPE_T_REQUEST_COMPUTATION 1
# define TYPE_T_REQUEST_FINISH 2

# define TYPE_T_RESPONSE_SUCCESS 0
# define TYPE_T_RESPONSE_FAILURE 1

# define TEXTURES_DIR "textures"
# define TEXTURES_FILENAME_SIZE 16

#endif