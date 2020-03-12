/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiri <abiri@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/07 14:41:56 by abenaiss          #+#    #+#             */
/*   Updated: 2020/03/04 11:29:58 by abiri            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJECTS_H
# define OBJECTS_H

# include "ft_maths.h"

# define TYPE_SPHERE 1
# define TYPE_CYLINDER 2
# define TYPE_PLANE 3
# define TYPE_CONE 4
# define TYPE_ELLIPSOID 5
# define TYPE_TRIANGLE 7
# define TYPE_PARABALOID 8
# define TYPE_HYPERBOILD 6
# define TYPE_FRACTAL 9
# define TYPE_HOLO_CUBE 10
# define TYPE_TORUS 11
# define TYPE_CUBE 12
# define TYPE_PARSE_COUNT 20
# define A abc[0]
# define B abc[1]
# define C abc[2]
# define TEXTURE_MODE_FIT 1
# define TEXTURE_MODE_CENTER 2
# define TEXTURE_MODE_CUT 4
# define TEXTURE_MODE_TRANSPARENCY 8
# define TEXTURE_MODE_REPEAT 16
# define C4 coeff[4]
# define C3 coeff[3]
# define C2 coeff[2]
# define C1 coeff[1]
# define C0 coeff[0]

typedef struct						s_color
{
	double							r;
	double							g;
	double							b;
}									t_color;

typedef double						t_intersection_function();
typedef t_color						t_procedural_texture_function(
									double x, double y);

typedef struct						s_limit
{
	t_coor							x_limit;
	t_coor							y_limit;
	t_coor							z_limit;
}									t_limit;

typedef struct						s_texture
{
	char							*texture_name;
	int								width;
	int								height;
	u_int32_t						*pixels;
}									t_texture;

typedef struct						s_material
{
	t_texture						*bump;
	t_texture						*texture;
	t_texture						*specular;
	t_texture						*transparency;
	t_texture						*reflection;
	t_procedural_texture_function	*proced_bump;
	t_procedural_texture_function	*proced_texture;
	t_procedural_texture_function	*proced_specular;
	t_procedural_texture_function	*proced_transparency;
	t_procedural_texture_function	*proced_reflection;
	double							refraction_index;
	double							reflection_index;
	double							transparency_index;
	t_coor							offset;
	double							scale;
	int								mode;
}									t_material;

typedef struct						s_sphere
{
	t_vector						normal;
	t_color							color;
	t_material						material;
	t_intersection_function			*function;
	t_vector						center;
	double							radius;
	t_vector						translation;
	t_vector						rotation;
	double							max_length;
	t_vector						cut_orientation;
	t_limit							limits;
	t_vector						axis;
}									t_sphere;

typedef struct						s_ellipsoid
{
	t_vector						normal;
	t_color							color;
	t_material						material;
	t_intersection_function			*function;
	t_vector						center;
	t_vector						translation;
	t_vector						rotation;
	t_vector						axis;
	t_limit							limits;
}									t_ellipsoid;

typedef struct						s_cylinder
{
	t_vector						normal;
	t_color							color;
	t_material						material;
	t_intersection_function			*function;
	t_vector						center;
	t_vector						axis;
	double							radius;
	t_vector						rotation;
	t_vector						translation;
	double							max_length;
	double							length;
	t_limit							limits;
	t_vector						limit;
}									t_cylinder;

typedef struct						s_hyperboloid
{
	t_vector						normal;
	t_color							color;
	t_material						material;
	t_intersection_function			*function;
	t_vector						center;
	t_vector						rotation;
	t_vector						translation;
	int								sheets;
	double							coefficient;
	t_limit							limits;
	t_vector						limit;
}									t_hyperboloid;

typedef struct						s_paraboloid
{
	t_vector						normal;
	t_color							color;
	t_material						material;
	t_intersection_function			*function;
	t_vector						center;
	t_vector						rotation;
	t_vector						translation;
	double							coefficient;
	t_limit							limits;
	t_vector						limit;
}									t_paraboloid;

typedef struct						s_holo_cube
{
	t_vector						normal;
	t_color							color;
	t_material						material;
	t_intersection_function			*function;
	t_vector						center;
	t_vector						translation;
	double							radius;
	t_limit							limits;
	t_vector						limit;
	double							coeff[5];
	double							soluces[4];
}									t_holo_cube;

typedef struct						s_torus
{
	t_vector						normal;
	t_color							color;
	t_material						material;
	t_intersection_function			*function;
	t_vector						translation;
	t_limit							limits;
	t_vector						center;
	t_vector						limit;
	double							b_radius;
	double							s_radius;
	double							coeff[5];
	double							soluces[4];
}									t_torus;

typedef struct						s_cube
{
	t_vector						normal;
	t_color							color;
	t_material						material;
	t_intersection_function			*function;
	t_vector						center;
	t_vector						translation;
	t_limit							limits;
	t_vector						bounds[2];
	t_vector						limit;
}									t_cube;

typedef struct						s_cone
{
	t_vector						normal;
	t_color							color;
	t_material						material;
	t_intersection_function			*function;
	t_vector						center;
	t_vector						axis;
	double							angle;
	double							tilt;
	t_vector						rotation;
	t_vector						translation;
	double							max_length;
	t_vector						length;
	t_limit							limits;
	t_vector						limit;
}									t_cone;

typedef struct						s_sides
{
	t_vector						u;
	t_vector						v;
	t_vector						w;
}									t_sides;

typedef struct						s_length
{
	double							u;
	double							v;
	double							w;
}									t_length;

typedef struct						s_plane
{
	t_vector						normal;
	t_color							color;
	t_material						material;
	t_intersection_function			*function;
	t_vector						center;
	t_vector						rotation;
	t_vector						translation;
	t_limit							limits;
	double							radius;
	t_length						length;
	t_sides							sides;
	double							side;
}									t_plane;

typedef struct						s_fractal
{
	t_vector						normal;
	t_color							color;
	t_material						material;
	t_intersection_function			*function;
	t_vector						center;
	double							max_iter;
	double							max_steps;
	double							bailout;
	double							power;
	double							zr;
	double							dr;
	double							r;
	double							phi;
	double							theta;
}									t_fractal;

typedef struct						s_triangle
{
	t_vector						normal;
	t_color							color;
	t_material						material;
	t_intersection_function			*function;
	t_vector						center;
	t_vector						rotation;
	t_vector						translation;
	t_limit							limits;
	t_vector						side[2];
	t_vector						limit;
}									t_triangle;

typedef struct						s_point
{
	t_vector						normal;
	t_color							color;
	t_material						material;
	t_intersection_function			*function;
}									t_point;

typedef union						u_object
{
	t_point							point;
	t_sphere						sphere;
	t_plane							plane;
	t_cylinder						cylinder;
	t_cone							cone;
	t_ellipsoid						ellipsoid;
	t_hyperboloid					hyperboloid;
	t_paraboloid					paraboloid;
	t_holo_cube						holo_cube;
	t_triangle						triangle;
	t_fractal						fractal;
	t_torus							torus;
	t_cube							cube;
}									t_object;

typedef struct						s_object_list
{
	t_object						object;
	int								type;
	struct s_object_list			*next;
}									t_object_list;

#endif
