/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/* ft_printf.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trus <trus@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 00:00:00 by trus             #+#    #+#              */
/*   Updated: 2026/04/25 00:00:00 by trus            ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */
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

static int	handle_format(char c, va_list *args)
{
	if (c == 's')
		return (print_str(va_arg(*args, char *)));
	if (c == 'd' || c == 'i')
		return (print_int(va_arg(*args, int)));
	if (c == 'c')
	{
		ft_putchar_fd((char)va_arg(*args, int), 1);
		return (1);
	}
	if (c == '%')
	{
		ft_putchar_fd('%', 1);
		return (1);
	}
	return (0);
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
			total += handle_format(*fmt, &args);
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
