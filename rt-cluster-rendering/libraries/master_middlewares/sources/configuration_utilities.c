/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configuration_utilities.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merras <merras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/02 04:32:39 by merras            #+#    #+#             */
/*   Updated: 2020/03/02 10:57:55 by merras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "master_middlewares.h"

static int	is_valid_ip(char *ip)
{
	int i;
	int counts[4];

	i = -1;
	ft_bzero(counts, sizeof(int[4]));
	while (ip[++i])
	{
		if ((ip[i] != '.' && !ft_isdigit(ip[i])) ||
(ip[i] == '.' && (!i || !ft_isdigit(ip[i - 1]) || !ft_isdigit(ip[i + 1]))))
			return (0);
		if (ip[i] != '.' && !counts[BYTE_BLOCK_LOCK])
		{
			counts[BYTE_BLOCK]++;
			counts[BYTE_BLOCK_LOCK] = 1;
		}
		if (ip[i] == '.' && !(counts[BYTE_BLOCK_LOCK] = 0))
		{
			counts[DOTS]++;
			if (i - counts[BYTE_BLOCK_OFFSET] > 3 ||
				ft_atoi(ip + counts[BYTE_BLOCK_OFFSET]) > 255)
				return (0);
			counts[BYTE_BLOCK_OFFSET] = i + 1;
		}
	}
	return (counts[DOTS] == 3 && counts[BYTE_BLOCK] == 4);
}

static char	**read_configuration_file(char *filename)
{
	char	buffer[BUFFER_SIZE];
	char	*raw_configuration;
	char	**lines;
	int		fd;

	if ((fd = open(filename, O_RDONLY)) == -1)
		return (NULL);
	raw_configuration = ft_strnew(0);
	ft_bzero(buffer, BUFFER_SIZE);
	while (read(fd, buffer, BUFFER_SIZE - 1) > 0)
	{
		raw_configuration = ft_strjoin_free(raw_configuration, buffer, 1);
		ft_bzero(buffer, BUFFER_SIZE);
	}
	lines = ft_strsplit(raw_configuration, '\n');
	free(raw_configuration);
	return (lines);
}

static int	get_program_fd(char *entry)
{
	char	*colon;

	if (!entry || !(colon = ft_strchr(entry, ':')))
		return (-1);
	*colon = 0;
	if (ft_strcmp(entry, "path"))
		return (-1);
	return (open(colon + 1, O_RDONLY));
}

static int	assign_nodes_addresses(char **configuration_lines,
				t_cluster *cluster)
{
	int i;

	i = -1;
	cluster->size = 0;
	while (configuration_lines[cluster->size + 1])
	{
		if (!is_valid_ip(configuration_lines[cluster->size + 1]))
			return (1);
		cluster->size++;
	}
	if (!(cluster->nodes = (t_slave *)malloc(sizeof(t_slave) * cluster->size)))
		return (1);
	while (++i < (int)cluster->size)
	{
		pthread_mutex_init(&cluster->nodes[i].mutex, NULL);
		cluster->nodes[i].ip = configuration_lines[i + 1];
		cluster->nodes[i].tasks_queue = t_dstruct_list_init();
		cluster->nodes[i].cluster = cluster;
	}
	return (0);
}

int			get_configuration(char *configuration_file, t_cluster *cluster)
{
	char **configuration_lines;

	if (!(configuration_lines = read_configuration_file(configuration_file)))
		return (1);
	if ((cluster->program = get_program_fd(configuration_lines[0])) == -1 ||
		assign_nodes_addresses(configuration_lines, cluster))
	{
		free_char_array(configuration_lines);
		return (1);
	}
	ft_strdel(&configuration_lines[0]);
	free(configuration_lines);
	return (0);
}
