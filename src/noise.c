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

// void        ft_create_noise()
// {
//     int i;
//     int j;

//     i = -1;
//     while(++i < NOISE_H)
//     {
//         j = -1;
//         while(++j < NOISE_W)
//             perlin_noise[i][j] = (rand() % 32767) / (double)32767;
//     }
// }

// double      ft_noise_interpolation(double x, double y)
// {
//     double fractx = x - (int)x;
//     double fracty = y - (int)y;

//     int x1 = ((int)x + NOISE_W) % NOISE_W;
//     int y1 = ((int)y + NOISE_H) % NOISE_H;
//     int x2 = (x1 + NOISE_W - 1) % NOISE_W;
//     int y2 = (y1 + NOISE_H - 1) % NOISE_H;

//     double value = 0.0;
//     value += fractx * fracty * perlin_noise[x1][y1];
//     value += (1 - fractx) * fracty * perlin_noise[y1][y2];
//     value += fractx * (1 - fracty) * perlin_noise[y2][x1];
//     value += (1 - fractx) * (1 - fracty) * perlin_noise[y2][x2];
//     return (value);
// }

// double      ft_turbulence(double x, double y, double size)
// {
//     double value = 0.0;
//     double first_size = size;
//     while(size >= 1)
//     {
//         value += ft_noise_interpolation(x / size, y / size) * size;
//         size /= 2.0;
//     }
//     return((128.0 * value / first_size) / 256.0);
// }

// t_color        ft_noise(t_cam *cam)
// {
//     double xPeriod = 1.0;
//     double yPeriod = 5.0;
//     double turbpower = 50.0;
//     double turbSize = 200.0;
//     double xyValue = cam->hit.uv.x * 10 * xPeriod / NOISE_W +
//         cam->hit.uv.y * 10 * yPeriod / NOISE_H + turbpower *
//         ft_turbulence(cam->hit.uv.x * 10, cam->hit.uv.y * 10, turbSize);
//     double sineValue = fabs(sin(xyValue * 3.14159));
//     double color_noise =  sineValue;
//     return((t_color){color_noise,color_noise,color_noise});
// }
