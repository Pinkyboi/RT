/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractals.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/07 18:54:44 by abiri             #+#    #+#             */
/*   Updated: 2020/02/15 20:23:50 by abenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

# define MAXIMUM_RAY_STEPS 2000
# define MINIMUM_RAY_DISTANCE MIN_D

int colors[] = {
	192,
873,
198251,
461421,
659055,
922226,
1185396,
1383030,
1646201,
1909371,
2107005,
2370175,
2633346,
2830980,
3094150,
3291785,
3554955,
3818125,
4015504,
4278674,
4541844,
4739478,
5002649,
5265819,
5463453,
5726624,
5989794,
6187428,
6450599,
6648233,
6911403,
7174573,
7372208,
7635378,
7898292,
8095927,
8359097,
8622267,
8819901,
9083072,
9280706,
9543876,
9807047,
10004681,
10267851,
10531022,
10728656,
10991826,
11254996,
11452631,
11715545,
11978715,
12176350,
12439520,
12637154,
12900325,
13163495,
13361129,
13624299,
13887470,
14085104,
14348274,
14611445,
14809079,
15072249,
15335420,
15334648,
15334132,
15333616,
15398636,
15398120,
15397604,
15397088,
15462108,
15461592,
15461076,
15526096,
15525580,
15525064,
15524548,
15589568,
15589053,
15588281,
15653301,
15652785,
15652269,
15651753,
15716773,
15716257,
15715741,
15780761,
15780245,
15779729,
15779213,
15844233,
15843717,
15843201,
15908222,
15907450,
15906934,
15906418,
15971438,
15970922,
15970406,
15969890,
16034910,
16034394,
16033878,
16098898,
16098382,
16097866,
16097350,
16162370,
16161855,
16161083,
16226103,
16225587,
16225071,
16224555,
16289575,
16289059,
16288543,
16353563,
16353047,
16352531,
16352015,
16417035,
16416519,
16416003,
16481024,
16218369,
15955715,
15693060,
15430406,
15167752,
14905097,
14642443,
14380045,
14117390,
13854736,
13592082,
13329427,
13132309,
12869654,
12607000,
12344602,
12081947,
11819293,
11556639,
11293984,
11031330,
10768676,
10506021,
10243623,
9980969,
9783850,
9521196,
9258541,
8995887,
8733233,
8470578,
8208180,
7945526,
7682871,
7420217,
7157563,
6894908,
6632254,
6435135,
6172737,
5910083,
5647428,
5384774,
5122120,
4859465,
4596811,
4334157,
4071758,
3809104,
3546450,
3283795,
3086677,
2824022,
2561368,
2298714,
2036315,
1773661,
1511007,
1248352,
985698,
723044,
460389,
197735
};

double	ft_mandelbrot_distance_estimator(t_vector pos, int *iter)
{
	t_vector z = pos;
	float dr = 1.0;
	float r = 0.0;
	int	Iterations = 100;
	double	Bailout = 2.0;
	double	Power = 2;
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
		dr =  r*Power*dr + 1.0;
		// scale and rotate the point
		float zr = r*r;
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
	color = ft_int_to_color(colors[(int)((double)status * 5.0) % 193]);
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