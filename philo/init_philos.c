/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philos.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huakbas <huakbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 16:06:06 by huakbas           #+#    #+#             */
/*   Updated: 2025/05/21 13:48:35 by huakbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	sleep_disorder(t_data *data, int time)
{
	long long	start;
	long long	now;

	start = ft_now();
	now = start;
	while ((now - start) < time)
	{
		now = ft_now();
		if (is_somone_dead_or_food_max_reached(data))
			return ;
	}
}

t_philo	**go_to_bath(t_philo **philos, t_data *data)
{
	int	i;

	if (data)
	{
		free(data->food_max_reached);
		free(data->who_is_dead);
		free(data);
		data = NULL;
	}
	if (!philos)
		return (NULL);
	i = 0;
	while (philos[i])
	{
		pthread_mutex_destroy(&philos[i]->mute_fork);
		free (philos[i]);
		i++;
	}
	free (philos);
	return (NULL);
}

static t_philo	**create_philos(t_philo **philos, t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		philos[i] = malloc(sizeof(t_philo));
		if (!philos[i])
		{
			ft_get_or_set_errors("Malloc failed in init_philo function (2)");
			return (go_to_bath(philos, data));
		}
		memset(philos[i], 0, sizeof(t_philo));
		if (i % 2 == 1)
			philos[i]->hungry = 1;
		philos[i]->name = i + 1;
		pthread_mutex_init(&philos[i]->mute_fork, NULL);
		philos[i]->data = data;
		philos[i]->time_to_die = data->time_to_die;
		philos[i]->time_to_eat = data->time_to_eat;
		philos[i]->time_to_sleep = data->time_to_sleep;
		philos[i]->start_time = data->start_time;
		i++;
	}
	return (philos);
}

static void	set_second_forks(t_philo **philos, t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		if (i == data->philo_count - 1)
		{
			if (data->philo_count == 1)
				philos[i]->mute_fork2 = NULL;
			else
				philos[i]->mute_fork2 = &philos[0]->mute_fork;
			philos[i]->hungry = 0;
		}
		else
			philos[i]->mute_fork2 = &philos[i + 1]->mute_fork;
		i++;
	}
}

t_philo	**init_philos(t_data *data)
{
	t_philo	**philos;

	philos = malloc(sizeof(t_philo *) * (data->philo_count + 1));
	if (!philos)
	{
		ft_get_or_set_errors("Malloc failed in init_philo function (1)");
		return (NULL);
	}
	memset(philos, 0, sizeof(t_philo *) * (data->philo_count + 1));
	philos = create_philos(philos, data);
	set_second_forks(philos, data);
	return (philos);
}
