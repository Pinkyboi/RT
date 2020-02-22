#include "master_middlewares.h"

char    *read_file(int fd, uint32_t *size)
{
    char    buffer[BUFFER_SIZE];
    struct  stat file_stat;
    int     offset;
    int     read_return;
    char    *content;

    if (fstat(fd, &file_stat) == -1)
        return (NULL);
    *size = file_stat.st_size;
    content = malloc(file_stat.st_size);
    offset = 0;
    while ((read_return = read(fd, buffer, BUFFER_SIZE - 1)) > 0)
    {
        ft_memcpy(content + offset, buffer, read_return);
        offset += read_return;
    }
    return (content);
}

