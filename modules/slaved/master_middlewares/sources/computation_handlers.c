#include "master_middlewares.h"

t_task  *create_task(t_task value)
{
    t_task *task;

    if (!(task = malloc(sizeof(t_task))))
        return (NULL);
    task->request = value.request;
    task->response = value.response;
    return (task);
}

void    init_computation(t_cluster *cluster)
{
    int status;

	log_info("initiating computation structure.");
    status = pthread_mutex_init(&cluster->computation.tasks_queue_mutex, NULL);
    status = pthread_mutex_init(&cluster->computation.done_queue_mutex, NULL);
    cluster->computation.tasks_queue = t_dstruct_list_init();
    cluster->computation.done_queue = t_dstruct_list_init();
}

void    queue_task(t_cluster *cluster, t_packet *request)
{
    pthread_mutex_lock(&cluster->computation.tasks_queue_mutex);
    queue_enqueue(&cluster->computation.tasks_queue,
        t_dstruct_create_node(create_task((t_task){request, NULL}), sizeof(t_task)));
    cluster->computation.size++;
    pthread_mutex_unlock(&cluster->computation.tasks_queue_mutex);
}

void	**fetch_computation_blob(t_cluster *cluster, int (*compare)(void *, void *))
{
	t_dstruct_node	*done_task;
	void			**blob;
	int				i;

	while (1)
	{
		pthread_mutex_lock(&cluster->computation.done_queue_mutex);
		if (cluster->computation.done_queue.size == cluster->size)
			break ;
		pthread_mutex_unlock(&cluster->computation.done_queue_mutex);
	}
	if (compare)
		list_sort(&cluster->computation.done_queue, compare);
	done_task = cluster->computation.done_queue.head;
	log_debug("done_queue size: %d.", cluster->computation.done_queue.size);
	if (!(blob = malloc(sizeof(void *) * (cluster->computation.done_queue.size + 1))))
		return (NULL);
	i = 0;
	while (done_task)
	{
		blob[i] = done_task->content;
		// log_debug("a task data: %*.*s.", CAST(done_task->content, t_packet *)->size,
			// CAST(done_task->content, t_packet *)->size, 
			// CAST(done_task->content, t_packet *)->data);
		done_task = done_task->next;
		i++;
	}
	blob[i] = NULL;
	return (blob);
}
