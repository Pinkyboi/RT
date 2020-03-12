# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: merras <merras@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/11/17 15:42:28 by abiri             #+#    #+#              #
#    Updated: 2020/03/04 20:55:05 by merras           ###   ########.fr        #
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
NAME = raytracer_master

FILENAMES = main.c \
			queuers.c

HEADER_FILE = master_raytracer.h

SRC_DIR = ./src
OBJ_DIR = ./obj
INC_DIR = ./inc

OBJ = $(addprefix $(OBJ_DIR)/, $(FILENAMES:.c=.o))

HEADER_FILES = $(addprefix $(INC_DIR)/, $(HEADER_FILE))

FLAGS = -Ofast -Wall -Werror -Wextra

INCLUDES = -I ./inc -I ../libraries/master_middlewares/includes -I ../libraries/centropy/includes -I ../libraries/packet/includes -I ../libraries/cdata_structures/shared_includes -I ../libraries/log/includes

# detecting the os and linking with the good minilibx

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
	LIBRARIES = -L ./minilibx -lmlx -lX11 -lXext -lm
else
	LIBRARIES = -lmlx -framework OpenGL -framework AppKit ../libraries/master_middlewares/master_middlewares.a ../libraries/centropy/centropy.a ../libraries/packet/packet.a ../libraries/cdata_structures/cdstruct.a ../libraries/log/log.a
endif

DELAY = 0

all : $(NAME)
$(NAME): $(OBJ)
	make -C ../libraries/packet
	make -C ../libraries/log
	make -C ../libraries/centropy
	make -C ../libraries/master_middlewares
	make -C ../libraries/cdata_structures
	@echo "$(CC) $(_lYELLOW)$(FLAGS)$(_END) $(_lCYAN)$(OBJ)$(_END)\n$(_lGREEN)$(LIBRARIES)$(_END) -I$(_RED)$(INC_DIR)$(_END)$(_RED)$(INCLUDES)$(_END) -o $(_lBLUE)$(NAME)$(_lEND)$(_RED)\n"
	@$(CC) $(FLAGS) $(OBJ) $(LIBRARIES) $(INCLUDES) -o $(NAME)

$(OBJ): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.c $(HEADER_FILES) | $(OBJ_DIR)
	$(CC) $(FLAGS) -c $< -o $@ -I $(INC_DIR) $(INCLUDES)

$(OBJ_DIR):
	@mkdir $(OBJ_DIR)

clean:
	@echo "\n$(_lCYAN)Makefile :$(_END) will delete $(_RED)$(OBJ) $(VIS_OBJ)$(_END)"
	@echo "starting in $(DELAY) sec, $(_RED)press Ctrl-c to abort$(_END)"
	@sleep $(DELAY)
	@rm -rf $(OBJ_DIR)

fclean: clean
	@echo "\n$(_lCYAN)Makefile :$(_END) will delete $(_RED)$(NAME)$(_END)"
	@echo "starting in $(DELAY) sec, $(_RED)press Ctrl-c to abort$(_END)"
	@sleep $(DELAY)
	@rm -f $(NAME)
re: fclean all
