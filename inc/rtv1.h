/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiri <abiri@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/17 16:13:19 by abiri             #+#    #+#             */
/*   Updated: 2020/01/11 00:05:21 by abiri            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RTV1_H
# define RTV1_H
# include "libft.h"
# include "parser.h"
# include "objects.h"
# include <mlx.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <time.h>
# include <pthread.h>
#include <stdio.h>

# define CAM_FOREWORD cam_vects[0]
# define CAM_RIGHT cam_vects[1]
# define CAM_UP cam_vects[2]
# define RATIO cam_utils[0]
# define HALF_HEIGHT cam_utils[1]	
# define HALF_WIDTH cam_utils[2]
# define WIN_WIDTH 1280
# define WIN_HEIGHT 720
# define NOISE_W 1000
# define NOISE_H 1000
# define MAX_D 1e30
# define MIN_D 1e-6
# define AMBIANT 0.5
# define FT_SQR(X) ((X) * (X))
# define FT_RAD(X) (((X) * M_PI) / 180)

# define EXIT 53
# define FOREWORD 12
# define BACKWARD 14
# define UP 13
# define LEFT 0
# define RIGHT 2
# define DOWN 1
# define AA 2
# define PIXEL_SIZE 5
# define NUM_THREAD 4
# define REFLECTION_DEPTH 6


double      perlin_noise[NOISE_H][NOISE_W];

typedef struct	s_img
{
	void		*mlx_win;
	void		*mlx_ptr;
	void		*mlx_img;
	void		*img_ptr;
	int			*data;
	int			size_l;
	int			bpp;
	int			endian;
}				t_img;

typedef struct	s_mlx
{
	void	*mlx_ptr;
	void	*win;
	t_img	img;
}				t_mlx;

typedef struct	s_light
{
	t_vector	center;
	t_vector	light_vect;
	t_vector	reflected_light_vect;
	t_color		color;
	double		intensity;
}				t_light;

typedef struct	s_light_list
{
	t_light				light;
	struct s_light_list	*next;
}				t_light_list;

typedef struct			s_intersection
{
	double	soluces[2];
	t_vector	position;
	t_vector	normal;
	t_color		color;
	double		reflection;
}						t_intersection;

typedef struct			s_cam
{
	t_vector		position;
	t_vector		look_at;
	t_vector		ray_direction;
	t_intersection	hit;
	t_vector		translation;
	t_vector		bottom_left;
	t_vector		w_scalar;
	t_vector		h_scalar;
	double			fov;
}						t_cam;

typedef	struct			s_actions
{
	char			mouvement;
}						t_actions;

typedef	struct	s_rtv
{
	t_cam			cam;
	t_object_list	*objects;
	t_object_list	*last_object;
	t_light_list	*lights;
	t_light_list	*last_light;
	t_actions		actions;
	t_mlx			mlx;
	double			min;
	double			row;
	double			column;
	double			min_w;
	double			max_w;
	int				effects;
	int				render_offset;
	int				render_y_offset;
	int				anti_aliasing;
	int				pixel_size;
}				t_rtv;

typedef int		t_xml_element(t_xml_tag *tag, t_rtv *env);

typedef	struct	s_xml_element_parse
{
	t_xml_element	*function;
	char			*type_name;
}				t_xml_element_parse;

/*
**  FUNCTIONS TO ADD SHAPES TO THE LIST
*/
int				ft_add_sphere(t_xml_tag *tag, t_rtv *env);
int				ft_add_cylinder(t_xml_tag *tag, t_rtv *env);
int				ft_add_plane(t_xml_tag *tag, t_rtv *env);
int				ft_add_disk(t_xml_tag *tag, t_rtv *env);
int				ft_add_cone(t_xml_tag *tag, t_rtv *env);
int				ft_add_demi_sphere(t_xml_tag *tag, t_rtv *env);
int				ft_add_ellipsoid(t_xml_tag *tag, t_rtv *env);
int				ft_add_hyperboloid(t_xml_tag *tag, t_rtv *env);
int				ft_add_paraboloid(t_xml_tag *tag, t_rtv *env);
int				ft_add_triangle(t_xml_tag *tag, t_rtv *env);
/*
**  FUNCTIONS TO CALCULATE INTERSECTION FOR EVERY SHAPE 
*/
double			ft_sphere_intersection(t_cam *cam,
		t_sphere *sphere, double min);
double			ft_plane_intersection(t_cam *cam,
        t_plane *plane, double min);
double			ft_cylinder_intersection(t_cam *cam,
		t_cylinder *cylinder, double min);
double			ft_cone_intersection(t_cam *cam,
		t_cone *cone, double min);
double			ft_ellipsoid_intersection(t_cam *cam,
		t_ellipsoid *ellipsoid, double min);
double          ft_hyperboloid_intersection(t_cam *cam,
		t_hyperboloid *hyperboloid, double min);
double			ft_paraboloid_intersection(t_cam *cam,
		t_paraboloid *paraboloid, double min);
double			ft_triangle_intersection(t_cam *cam,
        t_triangle *triangle, double min);
/*
**  FUNCTIONS TO CALCULATE NORMAL OF EVERY SHAPE 
*/
void			ft_sphere_normal(t_cam *cam,
        t_sphere *sphere, double distance);
void			ft_cylinder_normal(t_cam *cam,
        t_cylinder *cylinder, double distance);
void			ft_cone_normal(t_cam *cam,
        t_cone *cone, double distance);
void	        ft_ellipsoid_normal(t_cam *cam,
        t_ellipsoid *ellipsoid, double distance);
void            ft_hyperboloid_normal(t_cam *cam,
        t_hyperboloid *hyperboloid, double distance);
void            ft_paraboloid_normal(t_cam *cam,
        t_paraboloid *paraboloid, double distance);
/*
**  FUNCTIONS TO CALCULATE CAPPED OBJECTS AND LIMITED OBJECTS
*/
void			ft_define_limits(t_xml_tag *tag,
	t_limit *limit, int *status);
t_coor			ft_swap_limits(t_coor limits);
void            ft_sphere_cut(t_rtv *env,
	t_xml_tag *tag, t_object *object, int *status);
void			ft_cylinder_cut(t_rtv *env,
	t_xml_tag *tag, t_object *object, int *status);
void			ft_cone_cut(t_rtv *env,
	t_xml_tag *tag, t_object *object, int *status);
void			ft_demi_sphere_cut(t_rtv *env,
	t_xml_tag *tag, t_object *object, int *status);
void			ft_sides_handle(t_xml_tag *tag,
	t_object *object, int *status);
double		ft_sphere_limit(t_sphere sphere, t_cam cam);
double		ft_cone_limit(t_cone cone, t_cam cam);
double		ft_cylinder_limit(t_cylinder cylinder, t_cam cam);

/*
**	OTHER FUNCTION
*/

double			ft_check_intersection(t_rtv rtv);
double			ft_calculate_shadow(t_rtv rtv,
		double intersection_dist, t_light light);
double			ft_clamp_min(int min, double value);
double			ft_clamp_max(int max, double value);
double			ft_clamp_min_max(int min, int max, double value);
double			ft_choose_intersection(t_object_list *object_node,
		t_rtv *rtv, double *min);
int				ft_light_push(t_rtv *env, t_light light);
int				ft_load_shapes(t_xml_data *data, t_rtv *env);
int				ft_rgb_to_int(t_color color);
int				ft_key_stroke(int key, t_rtv *rtv);
t_color			ft_add_colors(t_color first, t_color second);
t_color			ft_scale_colors(t_color first, double scalar);
t_color			ft_diffuse(t_light light, t_vector normal, t_color color);
t_color			ft_specular(t_light light, t_vector normal);
void			ft_create_ray(t_rtv *rtv, int sample);
void			ft_init_cam(t_cam *cam);
void			ft_map_coordinates(t_rtv *rtv);
void			ft_init_cam(t_cam *cam);
void			ft_print_vect(t_vector v, char *name);
void			ft_ray_shooter(t_rtv *rtv);
void			ft_intersection_position(t_cam *cam, double first_intersection);
void			ft_put_pixel(t_rtv *rtv, int color);
void			ft_init_win(t_rtv *rtv);
void			ft_reflected_light_ray(t_cam cam, t_light *light, t_vector normal);
t_color			ft_parse_color(char *string, int *status);
t_vector				ft_parse_vector(char *string, int *status);
double			ft_atof(char *string, int *size);

int				ft_int_len(char *string);
t_color			ft_parse_hex(char *string, int *status);

t_xml_element	*ft_get_tag_parsing_function(char *name);
int				ft_add_light(t_xml_tag *tag, t_rtv *env);

int				ft_load_camera(t_xml_tag *tag, t_rtv *env);
int				ft_object_push(t_rtv *env, t_object object,
		int type);
size_t			ft_escape_whitespaces(char *str);
t_vector		ft_rotate_vector(t_vector a, t_vector angles);


t_color			ft_gray_filter(t_color color);
t_color			ft_negatif_filter(t_color color);
t_color			ft_sepia_filter(t_color color);

int				ft_antialiasing(t_rtv *rtv, t_vector normal, t_color color);

t_color			ft_mix_colors(t_rtv *rtv, t_vector normal, t_color color);

t_coor			ft_parse_coor(char *string, int *status);

t_plane			ft_define_plane(t_vector center, t_vector normal,
					t_color color, double radius);


t_color        	ft_noise(t_rtv rtv, t_color color);
void        	ft_create_noise(void);


t_light_list	*copy_lights(t_light_list* head);
t_object_list	*copy_objects(t_object_list* head);

int				ft_diff_color(t_color c1, t_color c2);

t_color			ft_cartoon_filter(t_rtv rtv, t_object object, t_color color);
t_color			ft_select_filter(t_rtv rtv, t_object object, t_color color);
t_color			ft_assign_color(double r, double g, double b);
int				ft_check_min_distance(double *x1, double x2, double min);
t_color			ft_reflect_ray(t_rtv rtv, t_color *color, int depth);

#endif
