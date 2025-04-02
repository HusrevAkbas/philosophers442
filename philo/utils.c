/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huakbas <huakbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 22:15:34 by husrevakbas       #+#    #+#             */
/*   Updated: 2025/04/02 14:20:11 by huakbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_get_timestamp(long long old)
{
	return (ft_now() - old);
}

long long	ft_now(void)
{
	t_timeval	now;

	if ( gettimeofday(&now, NULL) == -1)
	{
		ft_get_or_set_errors("Can not get current time value\n");
		return (0);
	}
	return (now.tv_sec * 1000 + now.tv_usec / 1000);
}

void	*ft_free_many(void *one, void *two, void *three, char *message)
{
	if (one)
		free (one);
	if (two)
		free (two);
	if (three)
		free (three);
	if (message)
		ft_get_or_set_errors(message);
	return (NULL);
}
