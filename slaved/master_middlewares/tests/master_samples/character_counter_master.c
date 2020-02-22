#include "master_middlewares.h"
#include <errno.h>

# define COMPUTATION_SIZE 75

char    *file_partial_read(int fd, int *size)
{
    char *content;
    int ret;
    int remaining;

    content = malloc(*size + 1);
    ft_bzero(content, *size + 1);
    remaining = *size;
    while (1)
    {
        ret = read(fd, content, remaining);
        if (!ret)
        {
            *size = ft_strlen(content);
            return (content);
        }
        if (ret == -1)
            return (NULL);
        remaining -= ret;
        if (!remaining)
            return (content);
    }
}

int main(int argc, char **argv)
{
    t_cluster   cluster;
    int         fd;
    struct stat input_state;
    int         err;
    int         i;

    if (argc != 3)
        exit (1);

    if ((fd = open(argv[1], O_RDONLY)) == -1)
        exit (2);

    if (init_cluster(argv[2], &cluster))
        exit (3);
    printf("success: init_cluster()..\n");
    printf("--> %zu\n", cluster.size);
    i = -1;
    int quota = cluster.size > 1 ? COMPUTATION_SIZE / (cluster.size - 1) : COMPUTATION_SIZE;
    while (++i < cluster.size)
    {
        // constructing the task request
        t_packet request;
        request.type = TYPE_T_REQUEST_COMPUTATION;
        request.size = quota;
        request.data = file_partial_read(fd, (int *)&request.size);
        queue_task(&cluster, create_packet(request, NULL));
    }
    DEBUG("waiting for computation result..\n");
    while (cluster.computation.size != cluster.computation.done_queue.size)
        continue ;
    //collect the result
    t_dstruct_node *s;
    long long result;
    long long temp;
    s = cluster.computation.done_queue.head;
    while (s)
    {
        memcpy(&temp, CAST(s->content, t_task *)->response->data, sizeof(temp));
        result += temp;
        s = s->next;
    }
    printf("computation is done, result: %lld\n", result);
    while (1)
        continue;
}
