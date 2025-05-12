/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huakbas <huakbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 22:15:34 by husrevakbas       #+#    #+#             */
/*   Updated: 2025/05/12 15:48:41 by huakbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_get_timestamp(long long old)
{
	long long	now;

	now = ft_now();
	if (now == -1)
	{
		safe_print(NULL, ft_get_or_set_errors(NULL));
		return (-1);
	}
	return (now - old);
}

long long	ft_now(void)
{
	struct timeval	now;

	if (gettimeofday(&now, NULL) == -1)
	{
		ft_get_or_set_errors("Can not get current time value\n");
		return (-1);
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

int	safe_print(t_data *data, char *message)
{
	// pthread_mutex_lock(&philo->data->mute_print);
	if (!data)
	{
		if (message)
			printf("%s", message);
		// pthread_mutex_unlock(&philo->data->mute_print);
		data->return_code = 3;
		return (3);
	}
	data->timestamp = ft_get_timestamp(data->start_time);
	if (data->timestamp == -1)
	{
		// pthread_mutex_unlock(&philo->data->mute_print);
		data->return_code = 3;
		return (3);
	}
	printf("%5i %3d %s\n", data->timestamp, data->name, message);
	// pthread_mutex_unlock(&philo->data->mute_print);
	return (0);
}
