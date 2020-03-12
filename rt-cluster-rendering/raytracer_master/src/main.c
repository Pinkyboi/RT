/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merras <merras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/12 08:48:50 by abiri             #+#    #+#             */
/*   Updated: 2020/03/04 21:56:44 by merras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "master_raytracer.h"

int	ft_init_window(t_mlx *mlx)
{
	mlx->mlx_ptr = mlx_init();
	if (!(mlx->img.img_ptr = mlx_new_image(mlx->mlx_ptr, WIDTH, HEIGHT)))
		return (0);
	if (!(mlx->img.data = (int *)mlx_get_data_addr(mlx->img.img_ptr,
		&mlx->img.bpp, &mlx->img.size_l, &mlx->img.endian)))
		return (0);
	if (!(mlx->win = mlx_new_window(mlx->mlx_ptr, WIDTH,
		HEIGHT, "RAYTRACER MASTER")))
		return (0);
	mlx->img.height = HEIGHT;
	mlx->img.width = WIDTH;
	return (1);
}

int	ft_display_loop(void *content)
{
	t_master_env	*env;

	env = content;
	mlx_put_image_to_window(env->mlx.mlx_ptr,
		env->mlx.win, env->mlx.img.img_ptr, 0, 0);
	return (0);
}

int	tasks_sorting_middleware(void *t1, void *t2)
{
	int	offset1;
	int	offset2;

	ft_memcpy(&offset1, CAST(t1, t_task *)->request->data, sizeof(int));
	ft_memcpy(&offset2, CAST(t2, t_task *)->request->data, sizeof(int));
	return (offset1 > offset2);
}

int	ft_show_results(t_cluster *cluster, t_master_env *env)
{
	void	**results;
	int		i;
	int		offset;

	i = -1;
	offset = 0;
	results = fetch_computation_blob(cluster, tasks_sorting_middleware);
	if (results && results[0])
	{
		while (results[++i])
		{
			ft_memcpy((void *)env->mlx.img.data + offset,
				CAST((results[i]), t_task *)->response->data,
				CAST((results[i]), t_task *)->response->size);
			offset += CAST((results[i]), t_task *)->response->size;
		}
		mlx_put_image_to_window(env->mlx.mlx_ptr, env->mlx.win,
			env->mlx.img.img_ptr, 0, 0);
		free(results);
		return (1);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_master_env	env;
	t_cluster		cluster;

	if (argc != 3)
		return (1);
	if (!(ft_init_window(&env.mlx)))
		return (2);
	if ((init_cluster(argv[1], &cluster)))
		return (3);
	LOG_DEBUG(">>>> %d.", cluster.size);
	if (!ft_send_scene(&cluster, argv[2]))
		return (5);
	if (!ft_send_tasks(&cluster))
		return (-1);
	if (!ft_show_results(&cluster, &env))
		return (-1);
	ft_display_loop(&env);
	list_delete(&cluster.computation.done_queue, destroy_task);
	delete_nodes(cluster.nodes, cluster.size);
	mlx_loop(env.mlx.mlx_ptr);
	return (0);
}
