/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/* ft_atol.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trus <trus@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 00:00:00 by trus             #+#    #+#              */
/*   Updated: 2026/04/25 00:00:00 by trus            ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

long	ft_atol(const char *str)
{
	long	result;
	int		sign;

	sign = 1;
	result = 0;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
		result = result * 10 + (*str++ - '0');
	return (result * sign);
}
