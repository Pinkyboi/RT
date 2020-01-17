/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dump_bitmap.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiri <abiri@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/16 18:15:46 by abiri             #+#    #+#             */
/*   Updated: 2020/01/17 12:38:10 by abiri            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
# define BMP_MAGIC_HEADER 19778
# define BMP_INFO_HEADER_SIZE 40
# define BMP_PLANES_NUMBER 1
# define BMP_BITS_PER_PIXEL 32
# define BMP_INFO_HEADER_ADVANCED "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
# define BMP_ALL_HEADERS_SIZE 54

int	ft_write_header(int fd, t_img *image)
{
	uint16_t	magic;
	uint32_t	size;
	int			bytes_size;

	magic = BMP_MAGIC_HEADER;
	bytes_size = image->width * image->height * 4;
	size = BMP_ALL_HEADERS_SIZE + bytes_size;
	if (write(fd, &magic, 2) != 2)
		return (ERROR);
	if (write(fd, &size, 4) != 4)
		return (ERROR);
	if (write(fd, "\0\0\0\0", 4) != 4)
		return (ERROR);
	size = BMP_ALL_HEADERS_SIZE;
	if (write(fd, &size, 4) != 4)
		return (ERROR);
	return (SUCCESS);
}

int	ft_write_bitmap_info(int fd, t_img *image)
{
	uint16_t	int_2bytes;
	uint32_t	int_4bytes;

	int_4bytes = BMP_INFO_HEADER_SIZE;
	if (write(fd, &int_4bytes, 4) != 4)
		return (ERROR);
	int_4bytes = image->width;
	if (write(fd, &int_4bytes, 4) != 4)
		return (ERROR);
	int_4bytes = image->height;
	if (write(fd, &int_4bytes, 4) != 4)
		return (ERROR);
	int_2bytes = BMP_PLANES_NUMBER;
	if (write(fd, &int_2bytes, 2) != 2)
		return (ERROR);
	int_2bytes = BMP_BITS_PER_PIXEL;
	if (write(fd, &int_2bytes, 2) != 2)
		return (ERROR);
	if (write(fd, BMP_INFO_HEADER_ADVANCED, 24) != 24)
		return (ERROR);
	return (SUCCESS);
}

int	ft_write_pixels(int fd, t_img *image)
{
	int size;
	int	i;
	int j;

	size = image->width * image->height * 4;
	i = image->height - 1;
	while (i >= 0)
	{
		j = 0;
		while (j < image->width)
		{
			write(fd, &image->data[i * image->width + j], 4);
			j++;
		}
		i--;
	}
	/*if (write(fd, image->data, size) != size)
		return (ERROR);*/
	return (SUCCESS);
}

int	ft_dump_bitmap(char *filename, t_img *image)
{
	int fd;

	if ((fd = open(filename, O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO)) < 0)
		return (ERROR);
	if (!ft_write_header(fd, image))
		return (ERROR);
	if (!ft_write_bitmap_info(fd, image))
		return (ERROR);
	if (!ft_write_pixels(fd, image))
		return (ERROR);
	return (SUCCESS);
}
