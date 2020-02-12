# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abenaiss <abenaiss@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/09/22 15:53:41 by azarzor           #+#    #+#              #
#    Updated: 2019/12/20 17:06:03 by abenaiss         ###   ########.fr        #
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
AR = ar rc
NAME = libftmaths.a
FILENAMES = vectors.c\
			vectors2.c\
			rotation.c\
			compare.c
HEADER_FILE = ./inc/ft_maths.h
SRC_DIR = ./src
OBJ_DIR = ./obj
INC_DIR = ./inc
SRC = $(addprefix $(SRC_DIR)/, $(FILENAMES))
OBJ = $(addprefix $(OBJ_DIR)/, $(FILENAMES:.c=.o))
FLAGS = -Wall -Werror -Wextra
INCLUDES = -I ./inc
LIBRARIES = 
all : $(NAME)
$(NAME): $(OBJ)
	$(AR) $(NAME) $(OBJ)

$(OBJ): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.c $(HEADER_FILE) | $(OBJ_DIR)
	$(CC) $(FLAGS) -c $< -o $@ -I $(INC_DIR) $(INCLUDES)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all
