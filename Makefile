NAME	= minishell

CC		= cc
CFLAGS	= -Wall -Wextra -Werror
IFLAGS	= -Iincludes -Ilibft

SRCS	=	src/main.c \
			src/signals.c \
			src/env/env_init.c \
			src/env/env_utils.c \
			src/parsing/tokenizer.c \
			src/parsing/parser.c \
			src/parsing/quotes.c \
			src/parsing/expand.c \
			src/execution/execute.c \
			src/execution/pipes.c \
			src/execution/redirection.c \
			src/builtins/builtins.c \
			src/builtins/builtin_cd.c \
			src/builtins/builtin_echo.c \
			src/builtins/builtin_env.c \
			src/builtins/builtin_export.c \
			src/builtins/builtin_pwd.c \
			src/builtins/builtin_exit.c

OBJS	= $(SRCS:.c=.o)

LIBFT	= libft/libft.a

all: $(LIBFT) $(NAME)

$(LIBFT):
	make -C libft

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -Llibft -lft -lreadline -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

clean:
	make -C libft clean
	rm -f $(OBJS)

fclean: clean
	make -C libft fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
