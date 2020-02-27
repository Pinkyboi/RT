/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   noise.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/25 21:29:47 by abenaiss          #+#    #+#             */
/*   Updated: 2020/02/21 22:22:33 by abenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

#include "rtv1.h"
static int permutation[] = {151,160,137,91,90,15,
131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,151,
131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,151,
160,137,91,90,15};

double ft_interpostion(double a, double b, double c)
{
    return b + a * (c - b);
}

double ft_fade(double n)
{
    return n * n * n * (n * (n * 6 - 15) + 10);
}

double ft_perlin_grad(int hash, float x, float y, float z)
{
    switch(hash & 15)
    {
        case 0: return  x + y;
        case 1: return -x + y;
        case 2: return  x - y;
        case 3: return -x - y;
        case 4: return  x + z;
        case 5: return -x + z;
        case 6: return  x - z;
        case 7: return -x - z;
        case 8: return  y + z;
        case 9: return -y + z;
        case 10: return  y - z;
        case 11: return -y - z;
        case 12: return  y + x;
        case 13: return -y + z;
        case 14: return  y - x;
        case 15: return -y - z;
        default: return 0;
    }
}

#define  A_ sides[0]
#define  AA sides[1]
#define  AB sides[2]
#define  B_ sides[3]
#define  BA sides[4]
#define  BB sides[5]
double ft_cubic_interpolation(t_vector left_side, t_vector right_side, t_vector point)
{
    int sides[8];
    A_ = permutation[(int)right_side.x]+ right_side.y;
    AA = permutation[A_]+right_side.z;
    AB = permutation[A_+1]+right_side.z;
    B_ = permutation[(int)right_side.x+1]+right_side.y;
    BA = permutation[B_]+right_side.z;
    BB = permutation[B_+1]+right_side.z;
    return(ft_interpostion(left_side.z,
    ft_interpostion(left_side.y,
    ft_interpostion(left_side.x,
    ft_perlin_grad(permutation[AA  ], point.x, point.y, point.z),
    ft_perlin_grad(permutation[BA], point.x - 1, point.y, point.z)),
    ft_interpostion(left_side.x,
    ft_perlin_grad(permutation[AB], point.x  , point.y-1, point.z),
    ft_perlin_grad(permutation[BB], point.x-1, point.y-1, point.z))),
    ft_interpostion(left_side.y,
    ft_interpostion(left_side.x,
    ft_perlin_grad(permutation[AA+1], point.x, point.y, point.z-1),
    ft_perlin_grad(permutation[BA+1], point.x-1, point.y, point.z-1)),
    ft_interpostion(left_side.x,
    ft_perlin_grad(permutation[AB+1], point.x, point.y-1, point.z-1),
    ft_perlin_grad(permutation[BB+1], point.x-1, point.y-1, point.z-1)))));
}

double ft_noise_3(t_vector point)
{
    int         i;
    t_vector    right_side;
    t_vector    left_side;

    i = -1;
    right_side.x = (int)point.x % 256,
    right_side.y = (int)point.y % 256,
    right_side.z = (int)point.z % 256;
    point.x -= (int)(point.x);
    point.y -= (int)(point.y);
    point.z -= (int)(point.z);
    left_side.x = ft_fade(point.x);
    left_side.y = ft_fade(point.y);
    left_side.z = ft_fade(point.z);
    return (ft_cubic_interpolation(left_side, right_side, point));
}

double ft_turbulence(double x, double y, double z, double size)
{
  double value = 0.0, initialSize = size;

  while(size >= 1)
  {
    value += ft_noise_3((t_vector){x / size, y / size, z / size}) * size;
    size /= 2.0;
  }

  return(value / initialSize);
}

t_color         ft_pastel(double x, double y)
{
    double color_noise;
    t_color color;

    x *= 30;
    y *= 10;
    color_noise = ft_turbulence(x, y, 0, 60) * 20;
    color_noise = color_noise - (int)color_noise;
    if(color_noise > 0 && color_noise < 0.2)
        color = (t_color){232.0/255.0,187.0/255.0,201.0/255.0};
    else if(color_noise < 0.4)
        color = (t_color){154.0/255.0,62.0/255.0,130.0/255.0};
    else if(color_noise < 0.6)
        color = (t_color){140.0/255.0,209.0/255.0,224.0/255.0};
    else if(color_noise < 0.8)
        color = (t_color){34.0/255.0,74.0/255.0,142.0/255.0};
    else
        color = (t_color){213.0/255.0,119.0/255.0,61.0/255.0};
     return(color);
}

t_color        ft_marble(double x, double y)
{
    double color;
    double noiseValue;

    noiseValue = 0; 
    noiseValue = ft_turbulence(x * 20, y * 20, 0, 6) * 5.0;
    color = (sin((y + noiseValue * 100) * 2 * M_PI / 200.f) + 1) / 2.f;
    return((t_color){color , color, color});
}

t_color     ft_wood(double x, double y)
{
    t_color color;
    double x_factor;
    double y_factor;
    double noise_value;
    double wave_value;

    x *= 200;
    y *= 200;
    x_factor = (x - 512 / 2) / (double)(512);
    y_factor = (y - 512 / 2) / (double)(512);
    noise_value = sqrt(x_factor * x_factor + y_factor * y_factor)
        + 2.0 * ft_turbulence(x, y, 0, 20) / 256.0;
    wave_value = 128.0 * fabs(sin(2 * 30.0 * noise_value * 3.14159));
    color.r = (int)(80 + wave_value) / 255.0;
    color.g = (int)(30 + wave_value) / 255.0;
    color.b = 30 / 255.0;
  return(color);
}