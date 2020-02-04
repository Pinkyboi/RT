/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/07 14:41:56 by abenaiss          #+#    #+#             */
/*   Updated: 2020/02/01 20:36:26 by abenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #ifndef OBJECTS_H
// # define OBJECTS_H

# include "ft_maths.h"

# define TYPE_SPHERE 1
# define TYPE_CYLINDER 2
# define TYPE_PLANE 3
# define TYPE_CONE 4
# define TYPE_ELLIPSOID 5
# define TYPE_HYPERBOILD 6
# define TYPE_TRIANGLE 7
# define TYPE_PARABALOID 8
# define TYPE_PARSE_COUNT 18
# define A abc[0]
# define B abc[1]
# define C abc[2]

typedef double	t_intersection_function();

typedef struct	s_color
{
	double	r;
	double	g;
	double	b;
}				t_color;

typedef struct s_limit
{
	t_coor	x_limit;
	t_coor	y_limit;
	t_coor	z_limit;
}				t_limit;

typedef struct	s_sphere
{
	t_vector				normal;
	t_color					color;
	double					reflection;
	double					refraction;
	double					transparency;
	t_intersection_function	*function;
	t_vector				center;
	double					radius;
	t_vector				translation;
	t_vector				rotation;
	double					max_lenght;
	t_vector				cut_orientation;
	t_limit					limits;
	t_vector				axis;
}				t_sphere;

typedef struct	s_ellipsoid
{
	t_vector				normal;
	t_color					color;
	double					reflection;
	double					refraction;
	double					transparency;
	t_intersection_function	*function;
	t_vector				center;
	t_vector				translation;
	t_vector				rotation;
	t_vector				axis;
	t_limit					limits;
}				t_ellipsoid;

typedef struct	s_cylinder
{
	t_vector				normal;
	t_color					color;
	double					reflection;
	double					refraction;
	double					transparency;
	t_intersection_function	*function;
	t_vector				center;
	t_vector				axis;
	double					radius;
	t_vector				rotation;
	t_vector				translation;
	double					max_lenght;
	double					lenght;
	t_limit					limits;	
}				t_cylinder;

typedef struct	s_hyperboloid
{
	t_vector				normal;
	t_color					color;
	double					reflection;
	double					refraction;
	double					transparency;
	t_intersection_function	*function;
	t_vector				center;
	t_vector				rotation;
	t_vector				translation;
	int						sheets;
	double					coefficient;
	t_limit					limits;	
}				t_hyperboloid;


typedef struct	s_paraboloid
{
	t_vector				normal;
	t_color					color;
	double					reflection;
	double					refraction;
	double					transparency;
	t_intersection_function	*function;
	t_vector				center;
	t_vector				rotation;
	t_vector				translation;
	double					coefficient;
	t_limit					limits;	
}				t_paraboloid;

typedef struct	s_cone
{
	t_vector				normal;
	t_color					color;
	double					reflection;
	double					refraction;
	double					transparency;
	t_intersection_function	*function;
	t_vector				center;
	t_vector				axis;
	double					angle;
	double					tilt;
	t_vector				rotation;
	t_vector				translation;
	double					max_lenght;
	t_vector				lenght;
	t_limit					limits;	
}				t_cone;

typedef struct	s_sides
{
	t_vector u;
	t_vector v;
}				t_sides;

typedef struct	s_plane
{
	t_vector				normal;
	t_color					color;
	double					reflection;
	double					refraction;
	double					transparency;
	t_intersection_function	*function;
	t_vector				center;
	t_vector				rotation;
	t_vector				translation;
	t_limit					limits;
	double					radius;
	double				side;
	t_sides				sides;
}				t_plane;



typedef struct	s_triangle
{
	t_vector				normal;
	t_color					color;
	double					reflection;
	double					refraction;
	double					transparency;
	t_intersection_function	*function;
	t_vector				center;
	t_vector				rotation;
	t_vector				translation;
	t_limit					limits;
	t_vector				side[2];
}				t_triangle;

typedef struct	s_point
{
	t_vector				normal;
	t_color					color;
	double					reflection;
	double					refraction;
	double					transparency;
	t_intersection_function	*function;
}				t_point;

typedef union	u_object
{
	t_point			point;
	t_sphere		sphere;
	t_plane			plane;
	t_cylinder		cylinder;
	t_cone			cone;
	t_ellipsoid		ellipsoid;
	t_hyperboloid	hyperboloid;
	t_paraboloid	paraboloid;
	t_triangle		triangle;
}				t_object;

typedef struct	s_object_list
{
	t_object				object;
	int						type;
	struct s_object_list	*next;
}				t_object_list;
