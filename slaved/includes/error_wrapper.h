#ifndef ERROR_WRAPPER_H
# define ERROR_WRAPPER_H

# define ERROR_WRAPPER(cond) err ? err : cond

int		ft_perror(char *command, char *arg, int err);

/*
** ERRORS
*/

#endif