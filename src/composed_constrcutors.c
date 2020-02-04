/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   composed_constrcutors.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/01 20:17:16 by abenaiss          #+#    #+#             */
/*   Updated: 2020/02/01 20:22:25 by abenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int				ft_add_pill(t_xml_tag *tag, t_rtv *env)
{
	t_object	body;
	t_object	first_edge;
	int			status;

	status = 1;
	body.cylinder.center = ft_parse_vector(ft_xml_get_value(tag, "center",
				"(0,0,0)"), &status);
	body.cylinder.radius = ft_parse_float(ft_xml_get_value(tag, "radius",
				"5"), &status);
	body.cylinder.axis = ft_parse_vector(ft_xml_get_value(tag, "axis",
				"(0, -1, 0)"), &status);
	body.cylinder.color = ft_parse_color(ft_xml_get_value(tag, "color",
				"(255,255,255)"), &status);
	body.cylinder.rotation = ft_parse_vector(ft_xml_get_value(tag, "rotation",
			"(0,0,0)"), &status);
	body.cylinder.axis = ft_normalise_vector(ft_rotate_vector(
				body.cylinder.axis, body.cylinder.rotation));
	body.cylinder.max_lenght = ft_clip_min(1,
		ft_parse_float(ft_xml_get_value(tag, "lenght", "5"),& status));
	ft_add_material(tag, &body, &status);
	ft_define_limits(tag, &(body.cylinder.limits), &status);
	body.cylinder.function = &ft_cylinder_intersection;
	status &= ft_object_push(env, body, TYPE_CYLINDER);
	first_edge.sphere.center = ft_add_vector(ft_scale_vector(body.cylinder.axis,
		body.cylinder.max_lenght/2), body.cylinder.center);
	first_edge.sphere.radius = body.cylinder.radius;
	first_edge.sphere.color = body.cylinder.color;
	first_edge.sphere.limits = body.cylinder.limits;
	ft_add_material(tag, &first_edge, &status);
	first_edge.sphere.function = &ft_sphere_intersection;
	status &= ft_object_push(env, first_edge, TYPE_SPHERE);
	first_edge.sphere.center = ft_add_vector(ft_scale_vector(body.cylinder.axis,
	-body.cylinder.max_lenght/2), body.cylinder.center);
	status &= ft_object_push(env, first_edge, TYPE_SPHERE);
	return (status);
}

// int		ft_create_plane(t_sides sides, t_color color, t_vector center, t_rtv *env)
// {
// 	t_object object;

// 	object.plane.normal = ft_normalise_vector(ft_cross_product(sides.u, sides.v)); 
// 	object.plane.color = color; 
// 	object.plane.center = center;
// 	object.plane.function = &ft_plane_intersection;
// 	ft_object_push(env, object, TYPE_PLANE);
// }

int			ft_add_cube(t_xml_tag *tag, t_rtv *env)
{
	t_object sides[6];
	t_vector center;
	int i = -1;
	int status;

	status = 1;
	center = ft_parse_vector(ft_xml_get_value(tag, "center",
				"(0,0,0)"), &status);
	sides[0].plane.side = ft_clip_min(10, ft_parse_float(
				ft_xml_get_value(tag, "side", "-1"), &status));
	sides[0].plane.sides.u = ft_normalise_vector(ft_parse_vector(ft_xml_get_value(tag, "U", "(0,1,0)"), &status));
	sides[0].plane.sides.v = ft_normalise_vector(ft_parse_vector(ft_xml_get_value(tag, "V",
	"(1,0,0)"), &status));
	sides[0].plane.color = ft_parse_color(ft_xml_get_value(tag, "color",
				"(255,255,255)"), &status);
	sides[0].plane.radius = -1;
	sides[0].plane.normal = ft_normalise_vector(ft_cross_product(sides[0].plane.sides.u, sides[0].plane.sides.v));
	sides[0].plane.center = ft_add_vector(ft_scale_vector(sides[0].plane.normal, sides[0].plane.side), center);
	sides[0].plane.function = &ft_plane_intersection;
	ft_define_limits(tag, &(sides[0].plane.limits), &status);
	ft_add_material(tag, &sides[0], &status);
	while(++i < 6)
		sides[i] = sides[0];

	sides[1].plane.center = ft_add_vector(ft_scale_vector(sides[0].plane.normal, -sides[0].plane.side), center);
	sides[2].plane.sides.u = sides[0].plane.normal;
	sides[2].plane.sides.v = sides[0].plane.sides.v;
	sides[2].plane.normal = ft_normalise_vector(ft_cross_product(sides[2].plane.sides.u, sides[2].plane.sides.v));
	sides[2].plane.center = ft_add_vector(ft_scale_vector(sides[2].plane.normal, sides[2].plane.side), center);
	sides[3].plane.center = ft_add_vector(ft_scale_vector(sides[2].plane.normal, -sides[2].plane.side), center);
	sides[3].plane.normal = sides[2].plane.normal;


	sides[4].plane.sides.u = sides[0].plane.sides.v;
	sides[4].plane.sides.v = sides[4].plane.normal;
	sides[4].plane.normal = ft_normalise_vector(ft_cross_product(sides[4].plane.sides.u, sides[4].plane.sides.v));
	sides[4].plane.center = ft_add_vector(ft_scale_vector(sides[4].plane.normal, sides[4].plane.side), center);
	sides[5].plane.center = ft_add_vector(ft_scale_vector(sides[4].plane.normal, -sides[4].plane.side), center);
	sides[5].plane.normal = sides[4].plane.normal;

	i = -1;
	while(++i < 6)
		ft_object_push(env, sides[i], TYPE_PLANE);
	return(status);
}