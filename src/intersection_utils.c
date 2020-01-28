/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiri <abiri@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/26 07:22:58 by abenaiss          #+#    #+#             */
/*   Updated: 2020/01/26 22:37:10 by abiri            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

double			ft_choose_intersection(t_object_list *object_node,
					t_rtv *rtv, double *min)
{
	double			temp_min;

	temp_min = object_node->object.point.function(&(rtv->cam),
		&object_node->object.point, *min);
	if (temp_min)
	{
		rtv->cam.hit.object = &object_node->object;
		*min = temp_min;
	}
	return (temp_min);
}

double			ft_check_intersection(t_rtv rtv)
{
	t_object_list	*object_node;
	double			min;
	double			temp_min;

	min = MAX_D;
	object_node = rtv.objects;
	while (object_node)
	{
		temp_min = ft_choose_intersection(object_node, &rtv, &min);
		if (temp_min)
			min = temp_min;
		object_node = object_node->next;
	}
	if (min)
		return (min);
	return (0);
}

t_color			ft_get_node_color(t_rtv rtv, int depth)
{
	t_color	node_color;
	t_color	reflection_color;
	t_color reflection_and_node;
	t_color	refraction_color;

	node_color = ft_scale_colors(
		ft_mix_colors(&rtv, rtv.cam.hit.normal, rtv.cam.hit.color),
		rtv.cam.hit.reflection);
	reflection_color = ft_scale_colors(
		ft_reflect_ray(rtv, depth),
		1 - rtv.cam.hit.reflection);
	reflection_and_node = ft_add_colors(node_color, reflection_color);
	refraction_color = ft_refract_ray(rtv, depth);
	if (rtv.cam.hit.refraction == 0)
		return (reflection_and_node);
	return (ft_add_colors(ft_scale_colors(reflection_and_node,
		1.0 - rtv.cam.hit.transparency), ft_scale_colors(refraction_color,
		rtv.cam.hit.transparency)));
}

void			ft_color_best_node(t_rtv *rtv, t_color rgb)
{
	t_object_list	*object_node;
	t_object_list	*best_node;
	int				sample;
	t_color			node_color;

	sample = -1;
	while (++sample < rtv->anti_aliasing + 1)
	{
		object_node = rtv->objects;
		rtv->min = MAX_D;
		best_node = NULL;
		ft_create_ray(rtv, sample);
		while (object_node)
		{
			if (ft_choose_intersection(object_node, rtv, &rtv->min))
				best_node = object_node;
			object_node = object_node->next;
		}
		if (best_node)
		{
			node_color = ft_get_node_color(*rtv, 1);
			rgb = ft_add_colors(rgb, node_color);
		}
	}
	rgb = ft_scale_colors(rgb, (double)1 / (rtv->anti_aliasing + 1));
	if (best_node)
	{
		ft_put_pixel(rtv, ft_rgb_to_int(
			ft_select_filter(*rtv, best_node->object, rgb)));
	}
	else
		ft_put_pixel(rtv, 0x0);
}

void			ft_display_loading(t_rtv *rtv)
{
	int x;
	int y;
	int	max;

	y = rtv->scene.height - 5;
	x = 0;
	while (x < rtv->scene.width)
	{
		y = rtv->scene.height - 5;
		while (y < rtv->scene.height)
		{
			if (x >= 0 && x < rtv->scene.width &&
					y >= 0 && y < rtv->scene.height)
				rtv->mlx.img.data[(int)(y * rtv->scene.width + x)] = 0x0;
			y++;
		}
		x++;
	}
	y = rtv->scene.height - 5;
	x = 0;
	max = (double)rtv->scene.width - ((double)((rtv->render_y_offset) * rtv->pixel_size + (rtv->render_offset)) / (double)(rtv->pixel_size * rtv->pixel_size + rtv->pixel_size)) * (double)rtv->scene.width;
	max = (max == rtv->scene.width) ? 0 : max;
	while (x < max)
	{
		y = rtv->scene.height - 5;
		while (y < rtv->scene.height)
		{
			if (x >= 0 && x < rtv->scene.width &&
					y >= 0 && y < rtv->scene.height)
				rtv->mlx.img.data[(int)(y * rtv->scene.width + x)] = 0xFF00FF;
			y++;
		}
		x++;
	}
}

void			*ft_ray_loop(void *data)
{
	t_color	rgb;
	t_rtv	*rtv;

	rtv = data;
	rtv->column = rtv->render_y_offset;
	while (rtv->column <= rtv->scene.height)
	{
		rtv->row = rtv->min_w + rtv->render_offset;
		while (rtv->row <= rtv->max_w)
		{
			rgb = (t_color){0, 0, 0};
			ft_color_best_node(rtv, rgb);
			rtv->row += rtv->pixel_size;
		}
		rtv->column += rtv->pixel_size;
		ft_display_loading(rtv);
	}
	return (NULL);
}

void			ft_ray_shooter(t_rtv *rtv)
{
	pthread_t	thread[NUM_THREAD];
	t_rtv		rtv_cpy[NUM_THREAD];
	int			i;

	i = -1;
	rtv->effects = 0;
	while (++i < NUM_THREAD)
	{
		rtv_cpy[i] = *rtv;
		// rtv_cpy[i].lights = copy_lights(rtv->lights);
		rtv_cpy[i].min_w = (rtv->scene.width / NUM_THREAD) * i;
		rtv_cpy[i].max_w = (rtv->scene.width / NUM_THREAD) * (i + 1);
		pthread_create(&thread[i], NULL, ft_ray_loop, &rtv_cpy[i]);
	}
	while (i--)
		pthread_join(thread[i], NULL);
	if (rtv->render_y_offset >= 0)
	{
		if (rtv->render_offset > 0)
			rtv->render_offset--;
		else
		{
			rtv->render_y_offset--;
			if (rtv->render_y_offset > 0)
				rtv->render_offset = PIXEL_SIZE;
		}
	}
}
