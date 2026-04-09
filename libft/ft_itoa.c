#include "libft.h"

static int	num_digits(long n)
{
	int	digits;

	digits = (n <= 0) ? 1 : 0;
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
	char	*result;

	num = (long)n;
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
	while (digits-- > (n < 0 ? 1 : 0))
	{
		result[digits] = '0' + (num % 10);
		num /= 10;
	}
	return (result);
}
