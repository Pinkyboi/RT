# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/11/17 15:42:28 by abiri             #+#    #+#              #
#    Updated: 2020/01/07 18:55:07 by abenaiss         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

_lCYAN = \x1b[96m
_lYELLOW = \x1b[93m
_lGREEN = \x1b[92m
_lBLUE = \x1b[94m
_RED = \x1b[31m
_BOLD = \x1b[1m
_END = \x1b[0m
CC = gcc
NAME = rtv1

FILENAMES = cam_utils.c\
	colors.c\
	colors_calcul.c\
	colors_utils.c\
	basics_constructors.c\
	limited_constructors.c\
	constructors2.c\
	basics_intersection.c\
	intersection_utils.c\
	key_managing.c\
	loading_functions.c\
	main.c\
	normal_and_refraction.c\
	parser_main.c\
	parser_tools.c\
	parser_types.c\
	string_parsers.c\
	ft_atof.c\
	xml_parser_tools.c\
	filter.c\
	shapes_cut.c\
	shapes_limit.c\
	cut_helper.c\
	noise.c\
	copy_list.c\
	colors_operations.c\
	quadrics_intersection.c\
	quadrics_constructors.c\
	quadrics_normals.c


HEADER_FILE = rtv1.h

SRC_DIR = ./src
OBJ_DIR = ./obj
INC_DIR = ./inc

OBJ = $(addprefix $(OBJ_DIR)/, $(FILENAMES:.c=.o))

HEADER_FILES = $(addprefix $(INC_DIR)/, $(HEADER_FILE))

FLAGS = -Wall -Werror -Wextra

FTMATHS = ./libs/ft_maths

LIBFTMATHS = $(FTMATHS)/libftmaths.a

LIBFTDIR = ./libs/libft

LIBFT = $(LIBFTDIR)/libft.a

INCLUDES = -I ./inc -I $(LIBFTDIR) -I ./libs/ft_maths/inc

# detecting the os and linking with the good minilibx

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
	LIBRARIES = -L $(LIBFTDIR) -lft -L ./minilibx -lmlx -lX11 -lXext -L $(FTMATHS) -lftmaths -lm
else
	LIBRARIES = -L $(LIBFTDIR) -lft -lmlx -framework OpenGL -framework AppKit -L $(FTMATHS) -lftmaths
endif



DELAY = 0

all : $(NAME)
$(NAME): $(OBJ) $(LIBFT) $(LIBFTMATHS)
	@echo "$(CC) $(_lYELLOW)$(FLAGS)$(_END) $(_lCYAN)$(OBJ)$(_END)\n$(_lGREEN)$(LIBRARIES)$(_END) -I$(_RED)$(INC_DIR)$(_END)$(_RED)$(INCLUDES)$(_END) -o $(_lBLUE)$(NAME)$(_lEND)$(_RED)\n"
	@$(CC) $(FLAGS) $(OBJ) $(LIBRARIES) $(INCLUDES) -o $(NAME)

$(OBJ): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(FLAGS) -c $< -o $@ -I $(INC_DIR) $(INCLUDES)

$(OBJ_DIR):
	@mkdir $(OBJ_DIR)

$(LIBFTMATHS):
	@make -C $(FTMATHS)

$(LIBFT):
	@make -C $(LIBFTDIR)

clean:
	@echo "\n$(_lCYAN)Makefile :$(_END) will delete $(_RED)$(OBJ) $(VIS_OBJ)$(_END)"
	@echo "starting in $(DELAY) sec, $(_RED)press Ctrl-c to abort$(_END)"
	@sleep $(DELAY)
	@make -C $(LIBFTDIR) clean
	@make -C $(FTMATHS) clean
	@rm -rf $(OBJ_DIR)

fclean: clean
	@echo "\n$(_lCYAN)Makefile :$(_END) will delete $(_RED)$(NAME)$(_END)"
	@echo "starting in $(DELAY) sec, $(_RED)press Ctrl-c to abort$(_END)"
	@sleep $(DELAY)
	@make -C $(LIBFTDIR) fclean
	@make -C $(FTMATHS) fclean
	@rm -f $(NAME)
re: fclean all