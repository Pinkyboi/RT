/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/26 07:22:58 by abenaiss          #+#    #+#             */
/*   Updated: 2019/12/30 08:34:24 by abenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

double			ft_choose_intersection(t_object_list *object_node,
					t_rtv *rtv, double *min)
{
	double			temp_min;

	temp_min = object_node->object.point.function(&(rtv->cam),
		&object_node->object.point, min);
	if (temp_min)
		*min = temp_min;
	return (temp_min);
}

double			ft_check_intersection(t_rtv rtv,
			t_vector light_vector, t_vector intersection_point)
{
	t_object_list	*object_node;
	double			min;
	double			temp_min;

	rtv.cam.position = intersection_point;
	rtv.cam.ray_direction = light_vector;
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

void			ft_color_best_node(t_rtv *rtv, t_color rgb)
{
	t_object_list	*object_node;
	t_object_list	*best_node;
	int				sample;

	sample = -1;
	while (++sample < (AA + 1))
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
			rgb = ft_add_colors(rgb, ft_mix_colors(rtv,
				best_node->object.point.normal, best_node->object.point.color));
	}
	rgb = ft_scale_colors(rgb, (double)1 / (AA + 1));
	if (best_node)
		ft_put_pixel(rtv, ft_rgb_to_int(rgb));
}

void			*ft_ray_loop(void *data)
{
	t_color	rgb;
	t_rtv	*rtv;

	
	ft_create_noise();
	rtv = data;
	rtv->column = -1;
	while (++rtv->column < WIN_HEIGHT)
	{
		rtv->row = rtv->min_w -1;
		while (++rtv->row < rtv->max_w)
		{
			rgb = (t_color){0, 0, 0};
			ft_color_best_node(rtv, rgb);
		}
	}
	return (NULL);
}

// t_rtv copy_objects(t_rtv rtv)
// {
// 	t_object_list	*list_node;
// 	t_rtv			rtv_cpy;

// 	rtv_cpy = rtv;
// 	if (!(list_node = ft_memalloc(sizeof(t_object_list))))
// 		exit(-1);
// 	rtv_cpy.objects = list_node;
// 	t_object_list *head_cpy = rtv_cpy.objects;
// 	t_object_list *object_node = rtv->objects;
// 	while(object_node)
// 	{
// 		printf("HIII");
// 		head_cpy->object = object_node->object;
// 		head_cpy->type = object_node->type;
// 		head_cpy->next = ft_memalloc(sizeof(t_object_list));
// 		object_node = object_node->next;
// 		head_cpy = head_cpy->next;
// 	}
// 	head_cpy->next = NULL;

// 	return(rtv_cpy);
// }

t_object_list* copy_objects(t_object_list* head)
{
	t_object_list* current = head;
	t_object_list* newList = NULL;
	t_object_list* tail = NULL;

	while (current != NULL)
	{
		if (newList == NULL)
		{
			newList = malloc(sizeof(t_object_list));
			newList->object = current->object;
			newList->type = current->type;
			newList->next = NULL;
			tail = newList;
		}
		else
		{
			tail->next = malloc(sizeof(t_object_list));
			tail = tail->next;
			tail->object = current->object;
			tail->type = current->type;
			tail->next = NULL;
		}
		current = current->next;
	}

	return newList;
}

void* copy_lights(t_light_list* head)
{
	t_light_list* current = head;
	t_light_list* newList = NULL;
	t_light_list* tail = NULL;

	while (current != NULL)
	{
		if (newList == NULL)
		{
			newList = malloc(sizeof(*head));
			newList->light = current->light;
			newList->next = NULL;
			tail = newList;
		}
		else
		{
			tail->next = malloc(sizeof(*head));
			tail = tail->next;
			tail->light = current->light;
			tail->next = NULL;
		}
		current = current->next;
	}

	return newList;
}



void	ft_ray_shooter(t_rtv *rtv)
{
	pthread_t	thread[NUM_THREAD];
	t_rtv		rtv_cpy[NUM_THREAD];
	int			i;

	i = -1;
	while (++i < NUM_THREAD)
	{
		rtv_cpy[i] = *rtv;
		rtv_cpy[i].objects = copy_objects(rtv->objects);
		rtv_cpy[i].last_object = NULL;
		rtv_cpy[i].lights = copy_lights(rtv->lights);
		rtv_cpy[i].last_light = NULL;
		rtv_cpy[i].min_w = (WIN_WIDTH / NUM_THREAD) * i;
		rtv_cpy[i].max_w = (WIN_WIDTH / NUM_THREAD) * (i + 1);
		pthread_create(&thread[i], NULL, ft_ray_loop, &rtv_cpy[i]);
	}
	while (i--)
		pthread_join(thread[i], NULL);
}