/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   basics_constructors.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiri <abiri@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/18 02:09:05 by abiri             #+#    #+#             */
/*   Updated: 2020/01/30 13:58:12 by abiri            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_texture		*ft_get_texture(char *filename, t_rtv *env)
{
	t_texture	*result;

	env->textures.iterator = env->textures.first;
	while ((result = ttslist_iter_content(&env->textures)))
	{
		if (ft_strequ(filename, result->texture_name))
			return (result);
	}
	return (NULL);
}

t_texture		*ft_load_image(char *filename, t_rtv *rtv)
{
	t_texture	*result;
	int			fd;

	if ((fd = open(filename, O_RDONLY)) < 0)
		return (NULL);
	if (!(result = ft_memalloc(sizeof(t_texture))))
		return (NULL);
	if (read(fd, &result->width, 4) != 4)
		return (NULL);
	if (read(fd, &result->height, 4) != 4)
		return (NULL);
	if (!(result->pixels = malloc(sizeof(u_int32_t) *
		result->width * result->height)))
		return (NULL);
	read(fd, result->pixels, result->width *
		result->height * sizeof(u_int32_t));
	result->texture_name = filename;
	rtv->textures.push(&rtv->textures, result);
	return (result);
}

t_texture		*ft_load_texture(char *filename, t_rtv *env)
{
	t_texture	*result;

	if (!filename)
		return (NULL);
	result = ft_get_texture(filename, env);
	if (!result)
		result = ft_load_image(filename, env);
	return (result);
}

void			ft_add_material(t_xml_tag *tag, t_object *object, int *status, t_rtv *env)
{
	object->point.reflection = ft_clip_max(1,
		ft_parse_float(ft_xml_get_value(tag, "reflection", "1"), status));
	object->point.refraction = ft_clip_min(1,
		ft_parse_float(ft_xml_get_value(tag, "refraction", "1"), status));
	object->point.transparency = ft_clip_max(1,
		ft_parse_float(ft_xml_get_value(tag, "transparency", "0"), status));
	object->point.texture = ft_load_texture(ft_xml_get_value(tag, "texture", NULL), env);
	object->point.bump = ft_load_texture(ft_xml_get_value(tag, "bump", NULL), env);
	object->point.specular = ft_load_texture(ft_xml_get_value(tag, "specular", NULL), env);
}

int				ft_add_sphere(t_xml_tag *tag, t_rtv *env)
{
	t_object	object;
	int			status;

	status = 1;
	object.sphere.center = ft_parse_vector(ft_xml_get_value(tag, "center",
				"(0,0,0)"), &status);
	object.sphere.radius = ft_parse_float(ft_xml_get_value(tag, "radius",
				"5"), &status);
	object.sphere.color = ft_parse_color(ft_xml_get_value(tag, "color",
				"(255,255,255)"), &status);
	object.sphere.translation = ft_parse_vector(ft_xml_get_value(tag,
				"translation", "(0,0,0)"), &status);
	object.sphere.center = ft_add_vector(object.sphere.center,
			object.sphere.translation);
	ft_sphere_cut(env, tag, &object, &status);
	ft_add_material(tag, &object, &status, env);
	object.sphere.function = &ft_sphere_intersection;
	status &= ft_object_push(env, object, TYPE_SPHERE);
	return (status);
}

int				ft_add_cylinder(t_xml_tag *tag, t_rtv *env)
{
	t_object	object;
	int			status;

	status = 1;
	object.cylinder.center = ft_parse_vector(ft_xml_get_value(tag, "center",
				"(0,0,0)"), &status);
	object.cylinder.radius = ft_parse_float(ft_xml_get_value(tag, "radius",
				"5"), &status);
	object.cylinder.axis = ft_parse_vector(ft_xml_get_value(tag, "axis",
				"(0, -1, 0)"), &status);
	object.cylinder.color = ft_parse_color(ft_xml_get_value(tag, "color",
				"(255,255,255)"), &status);
	object.cylinder.rotation = ft_parse_vector(ft_xml_get_value(tag, "rotation",
			"(0,0,0)"), &status);
	object.cylinder.translation = ft_parse_vector(ft_xml_get_value(tag,
				"translation", "(0,0,0)"), &status);
	object.cylinder.center = ft_add_vector(object.cylinder.center,
			object.cylinder.translation);
	object.cylinder.axis = ft_normalise_vector(ft_rotate_vector(
				object.cylinder.axis, object.cylinder.rotation));
	ft_cylinder_cut(env, tag, &object, &status);
	ft_add_material(tag, &object, &status, env);
	object.cylinder.function = &ft_cylinder_intersection;
	status &= ft_object_push(env, object, TYPE_CYLINDER);
	return (status);
}

int				ft_add_plane(t_xml_tag *tag, t_rtv *env)
{
	t_object	object;
	int			status;

	status = 1;
	object.plane.center = ft_parse_vector(ft_xml_get_value(tag, "center",
				"(0,0,0)"), &status);
	object.plane.normal = ft_parse_vector(ft_xml_get_value(tag, "normal",
				"(0,0,0)"), &status);
	object.plane.color = ft_parse_color(ft_xml_get_value(tag, "color",
				"(255,255,255)"), &status);
	object.plane.rotation = ft_parse_vector(
				ft_xml_get_value(tag, "rotation", "(0,0,0)"), &status);
	object.plane.translation = ft_parse_vector(ft_xml_get_value(tag,
				"translation", "(0,0,0)"), &status);
	object.plane.center = ft_add_vector(object.plane.center,
			object.plane.translation);
	object.plane.normal = ft_rotate_vector(object.plane.normal,
			object.plane.rotation);
	object.plane.normal = ft_normalise_vector(object.plane.normal);
	object.plane.radius = ft_clip_min(-1, ft_parse_float(
				ft_xml_get_value(tag, "radius", "-1"), &status));
	ft_add_material(tag, &object, &status, env);
	object.plane.function = &ft_plane_intersection;
	status &= ft_object_push(env, object, TYPE_PLANE);
	return (status);
}
