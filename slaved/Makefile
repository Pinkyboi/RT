NAME = slaved

SRCS_DIR = sources
OBJS_DIR = build
INCLUDES = includes libraries/centropy/includes libraries/cdata_structures/shared_includes libraries/packet/includes
LIBS_DIR = libraries

SRCS =	slaved.c \
		_requests_handlers.c \
		_requests_middlewares.c \
		_log.c \
		error_handling.c

OBJS = $(SRCS:.c=.o)

SRCS_PATHS = $(addprefix $(SRCS_DIR)/, $(SRCS))
OBJS_PATHS = $(addprefix $(OBJS_DIR)/, $(OBJS))

FLAGS = -g #-Wall -Wextra -Werror
INCLUDES_FLAGS = $(addprefix -I, $(INCLUDES))

all: $(NAME)

$(NAME): $(OBJS_PATHS)
	make -C $(LIBS_DIR)/centropy
	make -C $(LIBS_DIR)/cdata_structures
	make -C $(LIBS_DIR)/packet
	gcc $(FLAGS) $(OBJS_PATHS) $(LIBS_DIR)/centropy/centropy.a $(LIBS_DIR)/cdata_structures/cdstruct.a $(LIBS_DIR)/packet/packet.a -lpthread -o $(NAME)

$(OBJS_PATHS): $(OBJS_DIR)/%.o : $(SRCS_DIR)/%.c | $(OBJS_DIR)
	gcc $(FLAGS) $(INCLUDES_FLAGS) -c $< -o $@

$(OBJS_DIR):
	mkdir $(OBJS_DIR)

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all