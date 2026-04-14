NAME	= libft.a

CC		= cc
CFLAGS	= -Wall -Wextra -Werror

SRCS	=	ft_strcmp.c \
			ft_strdup.c \
			ft_strlen.c \
			ft_strchr.c \
			ft_substr.c \
			ft_strjoin.c \
			ft_split.c \
			ft_itoa.c \
			ft_atoi.c \
			ft_atol.c \
			ft_putstr_fd.c \
			ft_putendl_fd.c \
			ft_putchar_fd.c \
			ft_memcpy.c \
			ft_isalpha.c \
			ft_isalnum.c \
			ft_printf.c

OBJS	= $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -I. -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
