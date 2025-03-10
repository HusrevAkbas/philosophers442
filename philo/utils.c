/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husrevakbas <husrevakbas@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 11:39:12 by husrevakbas       #+#    #+#             */
/*   Updated: 2025/03/10 13:24:28 by husrevakbas      ###   ########.fr       */
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
		ft_get_or_set_errors("Argument is greater than INT_MAX\n");
		return (0);
	}
	else if (num < INT_MIN)
	{
		ft_get_or_set_errors("Argument is smaller than INT_MIN\n");
		return (0);
	}
	return (num);
}

int	ft_atoi_secure(char *num)
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
	if (!ft_isdigit(*num))
		return ((long)ft_get_or_set_errors("Argument is not a number\n"));
	a = 0;
	while (ft_isdigit(*num))
	{
		b = *num - 48;
		a = 10 * a - b;
		num++;
	}
	return (check_overflow(is_n * a));
}
