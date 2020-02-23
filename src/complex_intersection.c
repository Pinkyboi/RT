/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complex_intersection.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 23:27:31 by abenaiss          #+#    #+#             */
/*   Updated: 2020/02/21 22:22:33 by abenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


/*		TORUS		*/
#include "rtv1.h"

t_vector			ft_torus_normal(t_cam *cam, t_torus *torus)
{
	t_vector	normal;
	t_vector	tmp;
	
	tmp = ft_add_vector(cam->ray_origin, ft_scale_vector(cam->ray_direction, cam->hit.soluces[0]));
	normal.x = 4 * tmp.x * (pow(tmp.x, 2) + pow(tmp.y, 2) + pow(tmp.z, 2) - pow(2.0, 2) - pow(5.0, 2));
	normal.y = 4 * tmp.y * (pow(tmp.x, 2) + pow(tmp.y, 2) + pow(tmp.z, 2) - pow(2.0, 2) + 2 * pow(5.0, 2));
	normal.z = 4 * tmp.z * (pow(tmp.x, 2) + pow(tmp.y, 2) + pow(tmp.z, 2) - pow(2.0, 2) - pow(5.0, 2));
	normal = ft_normalise_vector(normal);
	return(normal);
}

double			ft_torus_intersection(t_cam *cam, t_torus *torus, double min)
{
	double		c[5];
	double		x[4];
	int			res;
	int			i;
	double		t;
	
	t_vector	tmp;
	t_vector d = cam->ray_direction;
	t_vector o = ft_sub_vector(cam->ray_origin, ft_new_vector(0,0,0));

	double sr = 2.0;
	double br = 5.0;
	c[4] = pow(ft_dot_vector(d, d), 2);
	c[3] = 4.0 * (pow(d.x, 2) + pow(d.y, 2) + pow(d.z, 2)) * (o.x * d.x + o.y * d.y + o.z * d.z);
	c[2] = 2.0 * (pow(d.x, 2) + pow(d.y, 2) + pow(d.z, 2)) * (pow(o.x, 2) + pow(o.y, 2) + pow(o.z, 2) - (pow(sr, 2) + pow(br, 2))) +
			4.0 * pow((o.x * d.x + o.y * d.y + o.z * d.z), 2) + 4.0 * pow(br, 2) * d.y * d.y;
	c[1] = 4.0 * ((pow(o.x, 2) + pow(o.y, 2) + pow(o.z, 2)) - (pow(sr, 2) + pow(br, 2))) * (o.x * d.x + o.y * d.y + o.z * d.z) + 8.0 * pow(br, 2) * o.y * d.y;
	c[0] = pow((pow(o.x, 2) + pow(o.y, 2) + pow(o.z, 2) - (pow(sr, 2) + pow(br, 2))), 2) - 4.0 * (pow(br, 2)) * (pow (sr, 2) - pow(o.y, 2));
	i = -1;
	t = MAX_D;
	res = solve_quartic(c, x);
	if (res == 0)
		return 0;
	cam->hit.soluces[0] = ft_min_sol(x, res);
	if (cam->hit.soluces[0] > MIN_D && cam->hit.soluces[0] < min)
	{
		ft_intersection_position(cam, cam->hit.soluces[0]);
		torus->normal = ft_torus_normal(cam, torus);
		ft_get_hit_info(torus->normal, (t_point *)torus, cam);
		return(cam->hit.soluces[0]);
	}
	return (0);
}

/*		HOLLOW CUBE		*/

t_vector		h_cube_normal(t_cam *cam)
{
	t_vector	tmp;
	t_vector	normal;
	
	tmp = ft_add_vector(cam->ray_origin, ft_scale_vector(cam->ray_direction, cam->hit.soluces[0]));
	normal.x = 4 * FT_CUB(tmp.x) - 10 * tmp.x;
	normal.y = 4 * FT_CUB(tmp.y) - 10 * tmp.y;
	normal.z = 4 * FT_CUB(tmp.z) - 10 * tmp.z;
	normal = ft_normalise_vector(normal);
	return (normal);
}


double			ft_cylinder_intersection(t_cam *cam, t_cylinder *h_cube, double min)
{
	double s[5];
	double x[4];
	int res;
	t_vector	tmp;

	tmp = ft_sub_vector(cam->ray_origin, ft_new_vector(0,0,0));
	s[4] = pow(cam->ray_direction.x, 4) + pow(cam->ray_direction.y, 4) + pow(cam->ray_direction.z, 4);
	s[3] = 4.0 * (pow(cam->ray_direction.x, 3) * tmp.x
				+ pow(cam->ray_direction.y, 3) * tmp.y
				+ pow(cam->ray_direction.z, 3) * tmp.z);
	s[2] = 6.0 * (pow(cam->ray_direction.x, 2) * pow(tmp.x, 2)
				+ pow(cam->ray_direction.y, 2) * pow(tmp.y, 2)
				+ pow(cam->ray_direction.z, 2) * pow(tmp.z, 2))
			- 5.0 * ft_dot_vector(cam->ray_direction, cam->ray_direction);
	s[1] = 4.0 * (pow(tmp.x, 3) * cam->ray_direction.x
				+ pow(tmp.y, 3) * cam->ray_direction.y
				+ pow(tmp.z, 3) * cam->ray_direction.z) - 10.0
				* ft_dot_vector(cam->ray_direction, tmp);
	s[0] = pow(tmp.x, 4) + pow(tmp.y, 4) + pow(tmp.z, 4)
			- 5.0 * ft_dot_vector(tmp, tmp) + 11.8;

	res = solve_quartic(s, x);
	cam->hit.soluces[0] = ft_min_sol(x, res);
	if (cam->hit.soluces[0] > MIN_D && cam->hit.soluces[0] < min)
	{
		ft_intersection_position(cam, cam->hit.soluces[0]);
		h_cube->normal = h_cube_normal(cam);
		ft_get_hit_info(h_cube->normal, (t_point *)h_cube, cam);
		return (cam->hit.soluces[0]);
	}
	return (0);
}

/*		BOX		*/

void	ft_box_normal(t_cam *cam, t_box *box)
{	
	t_vector	c;
	t_vector	d;
	t_vector	p;
	t_vector	hit;
	double	bias;

	bias = 1.00001;
	ft_intersection_position(cam, cam->hit.soluces[0]);
	c = ft_scale_vector(ft_add_vector(box->bounds[0], box->bounds[1]), 0.5);
	d = ft_scale_vector(ft_add_vector(box->bounds[0], box->bounds[1]), 0.5);
	d.x = fabs(d.x) * bias;
	d.y = fabs(d.y) * bias;
	d.z = fabs(d.z) * bias;
	p = ft_sub_vector(cam->hit.position, c);
	box->normal = ft_normalise_vector(ft_new_vector(p.x / d.x, p.y / d.y, p.z / d.z));
	ft_get_hit_info(box->normal, (t_point *)box, cam);
}

double		ft_box_intersection(t_cam *cam, t_box *box, double mint)
{

	t_vector		t[3];
	int			sign[3];
	t_vector		bounds[2];

	t[2] = (t_vector){1.0 / cam->ray_direction.x, 1.0 / cam->ray_direction.y, 1.0 / cam->ray_direction.z};
	sign[0] = t[2].x < 0.00001;
	sign[1] = t[2].y < 0.00001;
	sign[2] = t[2].z < 0.00001;
	t[0].x = (box->bounds[sign[0]].x - cam->ray_origin.x) * t[2].x;
	t[1].x = (box->bounds[1 - sign[0]].x - cam->ray_origin.x) * t[2].x;
	t[0].y = (box->bounds[sign[1]].y - cam->ray_origin.y) * t[2].y;
	t[1].y = (box->bounds[1 - sign[1]].y - cam->ray_origin.y) * t[2].y;
	if ((t[0].x > t[1].y) || (t[0].y > t[1].x))
		return (0);
	t[0].x = (t[0].y > t[0].x) ? t[0].y : t[0].x;
	t[1].x = (t[1].y < t[1].x) ? t[1].y : t[1].x;
	t[0].z = (box->bounds[sign[2]].z - cam->ray_origin.z) * t[2].z;
	t[1].z = (box->bounds[1 - sign[2]].z - cam->ray_origin.z) * t[2].z;
	if ((t[0].x > t[1].z) || (t[0].z > t[1].x))
		return (0);
	t[0].x = (t[0].z > t[0].x) ? t[0].z : t[0].x;
	t[1].x = (t[1].z < t[1].x) ? t[1].z : t[1].x;

	if (ft_check_min_distance(&t[0].x, t[1].x, mint))
	{
		ft_intersection_position(cam, t[0].x);
		cam->hit.soluces[0] = t[0].x;
		return(t[0].x);
	}	
}
