/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   basics_intersection.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azarzor <azarzor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/19 18:58:17 by abenaiss          #+#    #+#             */
/*   Updated: 2020/01/25 20:14:12 by azarzor          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_color			ft_get_color(int x, int y)
{
	static int *pixels = NULL;
	static int	width;
	static int height;

	if(!pixels)
	{
		int fd;

		fd = open("texture.tex", O_RDONLY);
		read(fd, &width, 4);
		read(fd, &height, 4);
		printf("loaded image %d %d\n", width, height);
		pixels = malloc(sizeof(int) * width * height);
		read(fd, pixels, width * height * sizeof(int));
		close(fd);
		printf("loadedfile\n");
	}
	x %= width;
	y %= height;
	t_color color;

	color.r = (double)((pixels[y * width + x] >> 16) & 0xff) / 255.0;
	color.g = (double)((pixels[y * width + x] >> 8) & 0xff) / 255.0;
	color.b = (double)(pixels[y * width + x] & 0xff) / 255.0;
	return (color);
}

int				ft_check_min_distance(double *x1, double x2, double min)
{
	if ((x2 <= *x1 || *x1 < MIN_D) && x2 > MIN_D
			&& (x2 < min))
	{
		*x1 = x2;
		return (1);
	}
	else if ((*x1 < x2 || x2 < MIN_D) && *x1 > MIN_D
			&& (*x1 < min))
		return (1);
	else
		return (0);
}

double			ft_cylinder_intersection(t_cam *cam,
		t_cylinder *cylinder, double min)
{
	double		abc[3];
	double		delta;
	t_vector	dist;

	dist = ft_sub_vector(cam->ray_origin, cylinder->center);
	A = 1 - FT_SQR(ft_dot_vector(cam->ray_direction, cylinder->axis));
	B = 2 * (ft_dot_vector(cam->ray_direction, dist)
			- (ft_dot_vector(cam->ray_direction, cylinder->axis) *
				ft_dot_vector(dist, cylinder->axis)));
	C = ft_dot_vector(dist, dist)
		- (ft_dot_vector(dist, cylinder->axis) *
				ft_dot_vector(dist, cylinder->axis))
		- (cylinder->radius * cylinder->radius);
	delta = (B * B) - (4 * A * C);
	if (delta < 0)
		return (0);
	cam->hit.soluces[0] = (-B + sqrt(delta)) / (2 * A);
	cam->hit.soluces[1] = (-B - sqrt(delta)) / (2 * A);
	if (ft_check_min_distance(&cam->hit.soluces[0], cam->hit.soluces[1], min))
	{
		ft_cylinder_normal(cam, cylinder, cam->hit.soluces[0]);
		cam->hit.uv = ft_cart_to_cylinder(cam->hit.position, cylinder);
		cam->hit.color = ft_get_color(cam->hit.uv.x * 565, 700 - (cam->hit.uv.y * 700));
		// cam->hit.color = ft_cheeker_texture(cam->hit.uv.x,cam->hit.uv.y, 20);
	}
	else
		cam->hit.soluces[0] = 0;
	return (cam->hit.soluces[0]);
}

double			ft_sphere_intersection(t_cam *cam,
		t_sphere *sphere, double min)
{
	double	abc[3];
	double	delta;
	
	A = 1;
	B = 2 * ft_dot_vector(cam->ray_direction,
			ft_sub_vector(cam->ray_origin, sphere->center));
	C = FT_SQR(ft_vector_size(ft_sub_vector(cam->ray_origin, sphere->center)))
		- (sphere->radius * sphere->radius);
	delta = (B * B) - (4 * A * C);
	if (delta < 0)
		return (0);
	cam->hit.soluces[0] = (-B + sqrt(delta)) / 2;
	cam->hit.soluces[1] = (-B - sqrt(delta)) / 2;
	if (ft_check_min_distance(&cam->hit.soluces[0], cam->hit.soluces[1], min))
	{
		ft_sphere_normal(cam, sphere, cam->hit.soluces[0]);
		cam->hit.uv = ft_cart_to_sphere(cam->hit.position, sphere);
		// cam->hit.color = ft_cheeker_texture(cam->hit.uv.x,cam->hit.uv.y, 20);
		cam->hit.color = ft_get_color(cam->hit.uv.x * 700, 700 - (cam->hit.uv.y * 700));

	}
	else
		cam->hit.soluces[0] = 0;
	return (cam->hit.soluces[0]);
}

double			ft_plane_intersection(t_cam *cam, t_plane *plane, double min)
{
	double		i;
	t_vector	temp;

	i = ft_dot_vector(cam->ray_direction, plane->normal);
	if (fabs(i) > MIN_D)
	{
		temp = ft_sub_vector(plane->center, cam->ray_origin);
		cam->hit.soluces[0] = ft_dot_vector(temp, plane->normal) / i;
		if (cam->hit.soluces[0] < min && cam->hit.soluces[0] > MIN_D)
		{
			ft_intersection_position(cam, cam->hit.soluces[0]);
			if (plane->radius < 0 || plane->radius >=
				ft_vector_size(ft_sub_vector(plane->center, cam->hit.position)))
			{
				cam->hit.normal = (i > 0) ?
					ft_scale_vector(plane->normal, -1) : plane->normal;
				ft_get_hit_info(cam->hit.normal, (t_point*)plane, cam);
				return (cam->hit.soluces[0]);
			}
		}
	}
	return (0);
}

double			ft_triangle_intersection(t_cam *cam,
	t_triangle *triangle, double min)
{
	double		det;
	double		test[2];
	t_vector	temp[3];

	temp[0] = ft_cross_product(cam->ray_direction, triangle->side[1]);
	det = ft_dot_vector(triangle->side[0], temp[0]);
	if (det > MIN_D)
	{
		temp[1] = ft_sub_vector(cam->ray_origin, triangle->center);
		test[0] = ft_dot_vector(temp[1], temp[0]) / det;
		if (test[0] < MIN_D || test[0] > 1)
			return (0);
		temp[2] = ft_cross_product(temp[1], triangle->side[0]);
		test[1] = ft_dot_vector(cam->ray_direction, temp[2]) / det;
		cam->hit.soluces[0] = ft_dot_vector(triangle->side[1], temp[2]) / det;
		if (!(test[1] < MIN_D || test[0] + test[1] > 1.0)
			&& ((cam->hit.soluces[0]) < min && cam->hit.soluces[0] > MIN_D))
		{
			ft_get_hit_info(triangle->normal, (t_point*)triangle, cam);
			ft_intersection_position(cam, cam->hit.soluces[0]);
			return (cam->hit.soluces[0]);
		}
	}
	return (0);
}
