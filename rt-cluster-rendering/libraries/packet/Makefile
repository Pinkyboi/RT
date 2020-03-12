NAME = packet.a

SRCS =	packet.c \
		io_wrappers.c

OBJS = $(SRCS:.c=.o)

SRCS_DIR = sources
OBJS_DIR = build
INCLUDES = includes ../log/includes

SRCS_PATH = $(addprefix $(SRCS_DIR)/, $(SRCS))
OBJS_PATH = $(addprefix $(OBJS_DIR)/, $(OBJS))

FLAGS = -Wall -Wextra -Werror
INCLUDES_FLAGS = $(addprefix -I, $(INCLUDES))

all: $(NAME)

$(NAME): $(OBJS_PATH)
	ar rc $(NAME) $(OBJS_PATH)
	ranlib $(NAME)

$(OBJS_PATH): $(OBJS_DIR)/%.o : $(SRCS_DIR)/%.c | $(OBJS_DIR)
	gcc $(FLAGS) $(INCLUDES_FLAGS) -c $< -o $@

$(OBJS_DIR):
	mkdir $(OBJS_DIR)

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all
