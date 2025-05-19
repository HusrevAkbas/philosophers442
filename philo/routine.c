/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huakbas <huakbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:50:19 by huakbas           #+#    #+#             */
/*   Updated: 2025/05/19 12:42:46 by huakbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	find_first_second_forks(t_philo *philo, pthread_mutex_t **first,
	pthread_mutex_t **second)
{
	if (!philo->mute_fork2)
		return ;
	if (philo->mute_fork2 > &philo->mute_fork)
	{
		(*first) = &philo->mute_fork;
		(*second) = philo->mute_fork2;
	}
	else
	{
		(*first) = philo->mute_fork2;
		(*second) = &philo->mute_fork;
	}
}

static int	take_forks(t_philo *philo)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	first = &philo->mute_fork;
	second = philo->mute_fork2;
	find_first_second_forks(philo, &first, &second);
	pthread_mutex_lock(first);
	if (is_somone_dead_or_food_max_reached(philo->data))
		return (unlock_mutex_return_one(first, NULL));
	safe_print(philo, "has taken a fork");
	if (philo->mute_fork2)
	{
		pthread_mutex_lock(second);
		if (is_somone_dead_or_food_max_reached(philo->data))
			return (unlock_mutex_return_one(first, second));
	}
	else
	{
		while (!is_somone_dead_or_food_max_reached(philo->data))
			;
		return (unlock_mutex_return_one(first, NULL));
	}
	return (0);
}

static int	eat(t_philo *philo)
{
	if (is_somone_dead_or_food_max_reached(philo->data))
		return (unlock_mutex_return_one(&philo->mute_fork, philo->mute_fork2));
	safe_print(philo, "is eating");
	pthread_mutex_lock(&philo->data->mute_data);
	philo->last_meal = ft_now();
	pthread_mutex_unlock(&philo->data->mute_data);
	philo->food_counter++;
	sleep_disorder(philo->data, philo->data->time_to_eat);
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
	if (philo->timestamp == -1)
		return (1);
	safe_print(philo, "is sleeping");
	sleep_disorder(philo->data, philo->time_to_sleep);
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
		if (philo->timestamp == -1)
			return (NULL);
		safe_print(philo, "is thinking");
		usleep(200);
	}
	return (NULL);
}
