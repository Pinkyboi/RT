/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_maths.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/22 15:24:45 by azarzor           #+#    #+#             */
/*   Updated: 2020/01/02 11:09:55 by abenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MATHS_H
# define FT_MATHS_H
# include <math.h>
# include <stdlib.h>

# define FT_RAD(X) (((X) * M_PI) / 180)

typedef struct		s_vector
{
	double	x;
	double	y;
	double	z;
}					t_vector;

typedef struct		s_coor
{
	double	x;
	double	y;
}					t_coor;

t_vector			ft_new_vector(double x, double y, double z);
t_vector			ft_add_vector(t_vector u, t_vector v);
double				ft_vector_size(t_vector u);
t_vector			ft_scale_vector(t_vector u, double scale);
t_vector			ft_normalise_vector(t_vector u);
t_vector			ft_sub_vector(t_vector v1, t_vector v2);
t_vector			ft_mult_vector(t_vector v1, t_vector v2);
t_vector			ft_cross_product(t_vector a, t_vector b);
t_vector			ft_origin(t_vector *o);
t_vector			ft_div_vector(t_vector v1, t_vector v2);
double				ft_dot_vector(t_vector v1, t_vector v2);
t_vector			ft_rotate_vectorx(t_vector a, double angle);
t_vector			ft_rotate_vectory(t_vector a, double angle);
t_vector			ft_rotate_vectorz(t_vector a, double angle);
t_vector			ft_rotate_vector(t_vector a, t_vector angles);
int					ft_vector_null(t_vector v);
int					ft_vector_limit(t_vector v1, t_vector v2);

#endif
