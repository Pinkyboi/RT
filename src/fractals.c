/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractals.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiri <abiri@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/07 18:54:44 by abiri             #+#    #+#             */
/*   Updated: 2020/02/09 04:42:02 by abiri            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

# define MAXIMUM_RAY_STEPS 10000
# define MINIMUM_RAY_DISTANCE 10e-8

int colors[] = {
40402,
959165,
1877928,
2796691,
3715454,
4634217,
5552980,
6471743,
7390506,
8309269,
9228288,
9949696,
10736640,
11458048,
12244992,
12966400,
13753344,
14474752,
15261696,
15983104,
16770304,
16502276,
16299784,
16097292,
15894800,
15692308,
15424280,
15221788,
15019296,
14816804,
14614568,
13111097,
11673418,
10235739,
8732268,
7294589,
5856910,
4353439,
2915760,
147808
};

double	ft_mandelbrot_distance_estimator(t_vector pos, int *iter)
{
	t_vector z = pos;
	float dr = 1.0;
	float r = 0.0;
	int	Iterations = 1000;
	double	Bailout = 2.0;
	double	Power = 8;
	*iter = 0;
	for (int i = 0; i < Iterations ; i++) {
		r = ft_vector_size(z);
		if (r > Bailout)
		{
			*iter = i;
			break;
		}

		// convert to polar coordinates
		float theta = acos(z.z/r);
		float phi = atan2(z.y, z.x);
		dr =  r*r*r*r*r*r*r*r*Power*dr + 1.0;
		// scale and rotate the point
		float zr = r*r*r*r*r*r*r*r;
		theta = theta*Power;
		phi = phi*Power;
		// convert back to cartesian coordinates
		z = ft_scale_vector((t_vector){sin(theta)*cos(phi), sin(phi)*sin(theta), cos(theta)}, zr);
		z = ft_add_vector(z, pos);
	}
	return 0.5*log(r)*r/dr;
}

float trace(t_vector from, t_vector direction, int *status)
{
	float totalDistance = 0.0;
	int steps;
	float distance = INFINITY;
	*status = 0;
	for (steps=0; steps < MAXIMUM_RAY_STEPS; steps++) {
		t_vector p = ft_add_vector(from, ft_scale_vector(direction, totalDistance));
		distance = ft_mandelbrot_distance_estimator(p, status);
		// printf("distance : %f\n", distance);
		// distance = ft_sphere_distance(p);
		totalDistance += distance;
		if (distance < MINIMUM_RAY_DISTANCE)
			break ;
	}
	return 1.0-((float)steps)/((float)MAXIMUM_RAY_STEPS);
}

t_color			ft_int_to_color(int color)
{
	double r;
	double g;
	double b;

	r = (color >> 16) & 0xFF;
	g = (color >> 8) & 0xFF;
	b = color & 0xFF;
	r /= 255.0;
	g /= 255.0;
	b /= 255.0;
	return ((t_color){r, g, b});
}

double			ft_fractal_intersection(t_cam *cam,
		t_fractal *fractal, double min)
{
	int		status;
	int		nothing;
	double	distance;
	t_vector	pos;

	pos = ft_sub_vector(cam->ray_origin, fractal->center);
	distance = trace(pos, cam->ray_direction, &status);
	if (!status || distance > min)
		return (0);
	cam->hit.uv = (t_coor){0, 0};
	cam->hit.soluces[0] = distance;
	double dis = 1;
	pos = ft_add_vector(pos, ft_scale_vector(ft_normalise_vector(cam->ray_direction), dis));
	/*t_vector n = (t_vector){ft_mandelbrot_distance_estimator((t_vector){pos.x + dis, pos.y, pos.z}, &nothing)
		- ft_mandelbrot_distance_estimator((t_vector){pos.x - dis, pos.y, pos.z}, &nothing),
		ft_mandelbrot_distance_estimator((t_vector){pos.x, pos.y + dis, pos.z}, &nothing)
		-ft_mandelbrot_distance_estimator((t_vector){pos.x, pos.y - dis, pos.z}, &nothing),
		ft_mandelbrot_distance_estimator((t_vector){pos.x, pos.y, pos.z + dis}, &nothing)
		-ft_mandelbrot_distance_estimator((t_vector){pos.x, pos.y, pos.z - dis}, &nothing)};*/
	// printf("1 : (%f, %f, %f)\n", n.x, n.y, n.z);
	//n = ft_normalise_vector(n);
	t_vector n = (t_vector){0, 1, 0};
	// printf("2 : (%f, %f, %f)\n", n.x, n.y, n.z);
	// n = ft_scale_vector(n, -1);
	t_color color;

	//color = (t_color){1.0 - (double)status / 50.0, (double)status / 10.0, 1.0 - (double)status / 30.0};
	color = ft_int_to_color(colors[(int)((double)status * 5.0) % 40]);
	ft_get_hit_info(n, (t_point *)fractal, cam);
	// cam->hit.color = ft_scale_colors(color, (double)status / 20.0);
	cam->hit.position = (t_vector){100, 100, 100};
	cam->hit.color = color;
	return (distance);
}

int				ft_add_fractal(t_xml_tag *tag, t_rtv *env)
{
	t_object	object;
	int			status;

	status = 1;
	object.fractal.center = ft_parse_vector(ft_xml_get_value(tag, "center",
				"(0,0,0)"), &status);
	object.fractal.color = ft_parse_color(ft_xml_get_value(tag, "color",
				"(255,255,255)"), &status);
	object.fractal.normal = ft_normalise_vector(object.fractal.normal);
	ft_add_material(tag, &object, &status, env);
	object.fractal.function = &ft_fractal_intersection;
	status &= ft_object_push(env, object, TYPE_PLANE);
	return (status);
}