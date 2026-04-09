#include "libft.h"

static int	print_str(char *s)
{
	if (!s)
		s = "(null)";
	ft_putstr_fd(s, 1);
	return (ft_strlen(s));
}

static int	print_int(int n)
{
	char	*s;
	int		len;

	s = ft_itoa(n);
	if (!s)
		return (0);
	len = ft_strlen(s);
	ft_putstr_fd(s, 1);
	free(s);
	return (len);
}

int	ft_printf(const char *fmt, ...)
{
	va_list	args;
	int		total;

	va_start(args, fmt);
	total = 0;
	while (*fmt)
	{
		if (*fmt == '%' && *(fmt + 1))
		{
			fmt++;
			if (*fmt == 's')
				total += print_str(va_arg(args, char *));
			else if (*fmt == 'd' || *fmt == 'i')
				total += print_int(va_arg(args, int));
			else if (*fmt == 'c')
			{
				ft_putchar_fd((char)va_arg(args, int), 1);
				total++;
			}
			else if (*fmt == '%')
			{
				ft_putchar_fd('%', 1);
				total++;
			}
		}
		else
		{
			ft_putchar_fd(*fmt, 1);
			total++;
		}
		fmt++;
	}
	va_end(args);
	return (total);
}
