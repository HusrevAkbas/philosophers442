/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husrevakbas <husrevakbas@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 22:15:34 by husrevakbas       #+#    #+#             */
/*   Updated: 2025/03/14 14:36:24 by husrevakbas      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_get_timestamp(t_timeval tv)
{
	t_timeval	now;

	if ( gettimeofday(&now, NULL) == -1)
	{
		ft_get_or_set_errors("Can not get current time value\n");
		return (0);
	}
	return (((now.tv_sec - tv.tv_sec) * 1000000 - tv.tv_usec + now.tv_usec) / 1000);
}
int	ft_update_last_meal(t_timeval *tv)
{
	t_timeval	now;

	if ( gettimeofday(&now, NULL) == -1)
	{
		ft_get_or_set_errors("Can not get current time value\n");
		return (0);
	}
	tv->tv_sec = now.tv_sec;
	tv->tv_usec = now.tv_usec;
	return (0);
}
