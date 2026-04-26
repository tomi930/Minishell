/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/* ft_itoa.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trus <trus@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 00:00:00 by trus             #+#    #+#              */
/*   Updated: 2026/04/25 00:00:00 by trus            ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static int	num_digits(long n)
{
	int	digits;

	if (n <= 0)
		digits = 1;
	else
		digits = 0;
	while (n)
	{
		n /= 10;
		digits++;
	}
	return (digits);
}

char	*ft_itoa(int n)
{
	long	num;
	int		digits;
	int		neg;
	char	*result;

	num = (long)n;
	neg = (n < 0);
	digits = num_digits(num);
	result = malloc(digits + 1);
	if (!result)
		return (NULL);
	result[digits] = '\0';
	if (num < 0)
	{
		result[0] = '-';
		num = -num;
	}
	while (digits-- > neg)
	{
		result[digits] = '0' + (num % 10);
		num /= 10;
	}
	return (result);
}
