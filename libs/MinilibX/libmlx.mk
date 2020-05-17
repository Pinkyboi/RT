# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    libmlx.mk                                          :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pbondoer <pbondoer@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/12/27 20:07:03 by pbondoer          #+#    #+#              #
#    Updated: 2017/02/03 03:02:20 by pbondoer         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Base variables for miniLibX based projects
# Use with 'include /path/to/mlx.mk'
# Make sure to define L_MLX with the path to miniLibX first

OS			:= $(shell uname)

L_MLX		?= .

MLX_NAME	:= libmlx.a

MLX_CAPITAN	:= $(L_MLX)/elcapitan/
MLX_SIERRA	:= $(L_MLX)/sierra/
MLX_X11		:= $(L_MLX)/X11/

ifeq ($(OS), Linux)
MLX_DIR		:= $(MLX_X11)
MLX_LNK		:= -lXext -lX11
else
OSX			:= $(shell sw_vers -productVersion)

ifneq (,$(findstring 10.12,$(OSX)))
MLX_DIR		:= $(MLX_SIERRA)
else
MLX_DIR		:= $(MLX_CAPITAN)
endif

MLX_LNK		:= -framework OpenGL -framework AppKit
endif

MLX_LNK		+= -L $(MLX_DIR) -l mlx
MLX_INC		:= -I $(MLX_DIR)
MLX_LIB		:= $(addprefix $(MLX_DIR),$(MLX_NAME))

# Global variables for compilation

ifndef LIB_LNK
LIB_LNK		:=
LIB_INC		:=
endif

LIB_LNK		+= $(MLX_LNK)
LIB_INC		+= $(LIB_INC)
