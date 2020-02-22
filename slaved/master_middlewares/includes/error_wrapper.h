#ifndef ERROR_WRAPPER_H
# define ERROR_WRAPPER_H

# define ERROR_WRAPPER(cond) err ? err : cond

int		ft_perror(char *command, char *arg, int err);

/*
** ERRORS
*/

# define CONNECTION_ERROR 1

# define CONNECTION_ERROR_T "error establishing connection with master."


#endif