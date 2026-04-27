NAME	= minishell

CC		= cc
CFLAGS	= -Wall -Wextra -Werror
IFLAGS	= -Iincludes -Ilibft

SRCS	=	src/main.c \
			src/error.c \
			src/signals.c \
			src/env/env_init.c \
			src/env/env_set.c \
			src/env/env_utils.c \
			src/parsing/tokenizer.c \
			src/parsing/tokenizer_utils.c \
			src/parsing/tokenizer_read.c \
			src/parsing/parser.c \
			src/parsing/parser_utils.c \
			src/parsing/quotes.c \
			src/parsing/expand.c \
			src/parsing/expand_utils.c \
			src/parsing/expand_env.c \
			src/parsing/free_cmd.c \
			src/parsing/redirection_parse.c \
			src/parsing/heredoc.c \
			src/execution/find_path.c \
			src/execution/execute.c \
			src/execution/pipes.c \
			src/execution/pipes_utils.c \
			src/execution/redirection.c \
			src/builtins/builtins.c \
			src/builtins/builtin_cd.c \
			src/builtins/builtin_echo.c \
			src/builtins/builtin_env.c \
			src/builtins/export_print.c \
			src/builtins/builtin_export.c \
			src/builtins/builtin_unset.c \
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
