/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huakbas <huakbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 22:15:34 by husrevakbas       #+#    #+#             */
/*   Updated: 2025/05/06 15:29:38 by huakbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_get_timestamp(long long old)
{
	return (ft_now() - old);
}

long long	ft_now(void)
{
	struct timeval	now;

	if (gettimeofday(&now, NULL) == -1)
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

void	safe_print(t_philo *philo, char *message)
{
	philo->timestamp = ft_get_timestamp(philo->start_time);
	pthread_mutex_lock(&philo->data->mute_print);
	printf("%5i %3d %s\n", philo->timestamp, philo->name, message);
	pthread_mutex_unlock(&philo->data->mute_print);
}

int	is_somone_dead_or_food_max_reached(t_data *data)
{
	pthread_mutex_lock(&data->mute_data);
	if (*data->who_is_dead || data->philo_count
		== *data->food_max_reached)
	{
		pthread_mutex_unlock(&data->mute_data);
		return (1);
	}
	pthread_mutex_unlock(&data->mute_data);
	return (0);
}
