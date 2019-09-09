NAME=ft_irc
DEPS=libft/libft.a ft_printf/libftprintf.a
LIBFT_DIR=libft
INCLUDES=$(LIBFT_DIR)/includes
REL_DEPS=$(DEPS:%=$(LIBFT_DIR)/%)
CC=gcc
CFLAGS=-Wall -Wextra -Werror -I . -I $(INCLUDES) -O0 -g
SERVER_FILES=	main.c	clean_fd.c	x.c	srv_create.c	srv_accept.c		\
	client_read.c	client_write.c	ring_buffer.c	ring_buffer_util.c		\
	message_handler.c
CLIENT_FILES= main.c
SERVER_FILES := $(addprefix server_src/,$(SERVER_FILES))
CLIENT_FILES := $(addprefix client_src/,$(CLIENT_FILES))

SERVER_OBJ:=$(SERVER_FILES:server_src/%.c=build/server/%.o)
CLIENT_OBJ:=$(CLIENT_FILES:client_src/%.c=build/client/%.o)

$(NAME): server client

server: $(SERVER_OBJ) $(REL_DEPS)
	@gcc $(CFLAGS) -o $@ $^

client: $(CLIENT_OBJ) $(REL_DEPS)
	@gcc $(CFLAGS) -o $@ $^

$(REL_DEPS):
	@make -C $(dir $@)

build/server/%.o: server_src/%.c server_src/*.h
	@mkdir -p build/server
	@$(CC) $(CFLAGS) -c $< -o $@

build/client/%.o: client_src/%.c
	@mkdir -p build/client
	@$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME);

clean fclean re::
	@for file in $(dir $(REL_DEPS)) ; do $(MAKE) -C $$file $@ ; done

clean::
	@rm -rf build/

fclean:: clean
	@rm -rf $(NAME)

re:: fclean all

.PHONY: clean fclean re all $(REL_DEPS)
