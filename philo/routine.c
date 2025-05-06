/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huakbas <huakbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:50:19 by huakbas           #+#    #+#             */
/*   Updated: 2025/05/06 15:10:01 by huakbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	find_first_second_forks(t_philo *philo, pthread_mutex_t **first,
	pthread_mutex_t **second)
{
	if (philo->mute_fork2 > &philo->mute_fork)
	{
		(*first) = philo->mute_fork2;
		(*second) = &philo->mute_fork;
	}
	else
	{
		(*first) = &philo->mute_fork;
		(*second) = philo->mute_fork2;
	}
}

static int	take_forks(t_philo *philo)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	first = &philo->mute_fork;
	second = philo->mute_fork2;
	if (philo->data->philo_count == 0)
		find_first_second_forks(philo, &first, &second);
	pthread_mutex_lock(first);
	if (is_somone_dead_or_food_max_reached(philo->data))
	{
		pthread_mutex_unlock(first);
		return (1);
	}
	safe_print(philo, "has taken a fork 1");
	if (philo->mute_fork2)
	{
		pthread_mutex_lock(second);
		if (is_somone_dead_or_food_max_reached(philo->data))
		{
			pthread_mutex_unlock(first);
			pthread_mutex_unlock(second);
			return (1);
		}
		safe_print(philo, "has taken a fork 2");
	}
	else
	{
		pthread_mutex_unlock(first);
		while (!is_somone_dead_or_food_max_reached(philo->data))
			;
		return (1);
	}
	if (is_somone_dead_or_food_max_reached(philo->data))
	{
		pthread_mutex_unlock(first);
		pthread_mutex_unlock(second);
		return (1);
	}
	return (0);
}

static int	eat(t_philo *philo)
{
	safe_print(philo, "is eating");
	philo->last_meal = ft_now();
	philo->food_counter++;
	usleep(philo->time_to_eat * 1000);
	pthread_mutex_lock(&philo->data->mute_data);
	if (philo->food_counter == philo->data->food_max)
		*philo->data->food_max_reached += 1;
	pthread_mutex_unlock(&philo->data->mute_data);
	pthread_mutex_unlock(&philo->mute_fork);
	pthread_mutex_unlock(philo->mute_fork2);
	if (is_somone_dead_or_food_max_reached(philo->data))
		return (1);
	return (0);
}

static int	have_a_nice_sleep(t_philo *philo)
{
	if (is_somone_dead_or_food_max_reached(philo->data))
		return (1);
	philo->timestamp = ft_get_timestamp(philo->data->start_time);
	safe_print(philo, "is sleeping");
	usleep(philo->time_to_sleep * 1000);
	if (is_somone_dead_or_food_max_reached(philo->data))
		return (1);
	return (0);
}

void	*routine(void	*arg)
{
	t_philo	*philo;

	philo = arg;
	while (1)
	{
		if (philo->hungry)
		{
			if (is_somone_dead_or_food_max_reached(philo->data))
				return (NULL);
			if (take_forks(philo))
				return (NULL);
			if (eat(philo))
				return (NULL);
		}
		philo->hungry = 1;
		if (have_a_nice_sleep(philo))
			return (NULL);
		philo->timestamp = ft_get_timestamp(philo->data->start_time);
		safe_print(philo, "is thinking");
	}
	return (NULL);
}
