/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   material.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 23:50:16 by abenaiss          #+#    #+#             */
/*   Updated: 2020/02/28 23:50:18 by abenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static void		ft_material_texture(t_xml_tag *tag,
	t_object *object, t_rtv *env)
{
	ft_load_texture(
		ft_xml_get_value(tag, "texture_mapping", NULL),
		env, &object->point.material.texture,
		&object->point.material.proced_texture);
	ft_load_texture(
		ft_xml_get_value(tag, "bump_mapping", NULL),
		env, &object->point.material.bump,
		&object->point.material.proced_bump);
	ft_load_texture(
		ft_xml_get_value(tag, "specular_mapping", NULL),
		env, &object->point.material.specular,
		&object->point.material.proced_specular);
	ft_load_texture(
		ft_xml_get_value(tag, "transparency_mapping", NULL),
		env, &object->point.material.transparency,
		&object->point.material.proced_transparency);
	ft_load_texture(
		ft_xml_get_value(tag, "reflection_mapping", NULL),
		env, &object->point.material.reflection,
		&object->point.material.proced_reflection);
}

void			ft_add_material(t_xml_tag *tag,
	t_object *object, int *status, t_rtv *env)
{
	t_vector	offset;

	ft_bzero(&object->point.material, sizeof(t_material));
	object->point.material.reflection_index = ft_clip_max(1,
		ft_parse_float(ft_xml_get_value(tag, "reflection", "1"), status));
	object->point.material.refraction_index = ft_clip_min(1,
		ft_parse_float(ft_xml_get_value(tag, "refraction", "1"), status));
	object->point.material.transparency_index = ft_clip_max(1,
		ft_parse_float(ft_xml_get_value(tag, "transparency", "0"), status));
	ft_material_texture(tag, object, env);
	offset = ft_parse_vector(ft_xml_get_value(tag,
		"mapping_position", "(0, 0, 1)"), status);
	object->point.material.offset = (t_coor){offset.x, offset.y};
	object->point.material.scale = offset.z;
	object->point.material.mode = ft_get_texture_mapping_type(tag);
}
