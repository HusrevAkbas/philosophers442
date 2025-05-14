/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argument_check.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huakbas <huakbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 11:39:12 by husrevakbas       #+#    #+#             */
/*   Updated: 2025/05/14 13:30:59 by huakbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_isdigit(int c)
{
	if (c < '0' || c > '9')
		return (0);
	else
		return (1);
}

int	check_overflow(long num)
{
	if (num > INT_MAX)
	{
		ft_get_or_set_errors(NUMBER_GREATER_THAN_INT_MAX);
		return (0);
	}
	else if (num < INT_MIN)
	{
		ft_get_or_set_errors(NUMBER_LOWER_THAN_INT_MIN);
		return (0);
	}
	return (num);
}

int	check_non_numerical(char *num, int is_n)
{
	int	i;

	i = 0;
	while (num[i])
	{
		if (!ft_isdigit(num[i]))
		{
			ft_get_or_set_errors(NOT_A_NUMBER);
			return (1);
		}
		i++;
	}
	if (i > 10)
	{
		if (is_n == -1)
			ft_get_or_set_errors(NUMBER_GREATER_THAN_INT_MAX);
		else
			ft_get_or_set_errors(NUMBER_LOWER_THAN_INT_MIN);
		return (1);
	}
	return (0);
}

int	ft_atoi_safe(char *num)
{
	long	a;
	long	b;
	int		is_n;

	is_n = -1;
	while (*num == ' ' || (*num <= 13 && *num >= 9))
		num++;
	if (*num == '-' || *num == '+')
	{
		if (*num == '-')
			is_n *= -1;
		num++;
	}
	if (check_non_numerical(num, is_n))
		return (0);
	a = 0;
	while (ft_isdigit(*num))
	{
		b = *num - 48;
		a = 10 * a - b;
		num++;
	}
	return (check_overflow(is_n * a));
}

void	ft_check_args(char **argv)
{
	int	i;
	int	a;

	i = 1;
	while (argv[i])
	{
		a = ft_atoi_safe(argv[i]);
		if (a == 0 && ft_get_or_set_errors(NULL))
			printf("Argument %d: %s", i, ft_get_or_set_errors(NULL));
		else if (a < 0 || a == 0)
		{
			ft_get_or_set_errors(NEGATIVE_NUMBER);
			printf("Argument %d: %s", i, NEGATIVE_NUMBER);
		}
		else if (i == 1 && a > 1000)
		{
			ft_get_or_set_errors(BIG_NUMBER_FOR_PHLOS);
			printf("Argument %d: %s", i, BIG_NUMBER_FOR_PHLOS);
		}
		i++;
	}
}
