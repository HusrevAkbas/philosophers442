/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huakbas <huakbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:50:19 by huakbas           #+#    #+#             */
/*   Updated: 2025/05/09 15:30:02 by huakbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// static void	find_first_second_forks(t_philo *philo, pthread_mutex_t **first,
// 	pthread_mutex_t **second)
// {
// 	if (philo->mute_fork2 > &philo->mute_fork)
// 	{
// 		(*first) = &philo->mute_fork;
// 		(*second) = philo->mute_fork2;
// 	}
// 	else
// 	{
// 		(*first) = philo->mute_fork2;
// 		(*second) = &philo->mute_fork;
// 	}
// }

static int	take_forks(t_data *data)
{
	(void) data;
	// pthread_mutex_t	*first;
	// pthread_mutex_t	*second;

	// first = &philo->mute_fork;
	// second = philo->mute_fork2;
	// find_first_second_forks(philo, &first, &second);
	// pthread_mutex_lock(first);
	// if (is_somone_dead_or_food_max_reached(philo->data))
	// 	return (unlock_mutex_return_one(first, NULL));
	// safe_print(philo, "has taken a fork 1");
	// if (philo->mute_fork2)
	// {
	// 	pthread_mutex_lock(second);
	// 	if (is_somone_dead_or_food_max_reached(philo->data))
	// 		return (unlock_mutex_return_one(first, second));
	// 	safe_print(philo, "has taken a fork 2");
	// }
	// else
	// {
	// 	while (!is_somone_dead_or_food_max_reached(philo->data))
	// 		;
	// 	return (unlock_mutex_return_one(first, NULL));
	// }
	return (0);
}

static int	eat(t_data *data)
{
	// if (is_somone_dead_or_food_max_reached(philo->data))
	// 	return (unlock_mutex_return_one(&philo->mute_fork, philo->mute_fork2));
	safe_print(data, "is eating");
	// pthread_mutex_lock(&philo->data->mute_data);
	data->last_meal = ft_now();
	// pthread_mutex_unlock(&philo->data->mute_data);
	data->food_counter++;
	usleep(data->time_to_eat * 1000);
	// pthread_mutex_lock(&philo->data->mute_data);
	if (data->food_counter == data->food_max)
		data->food_max_reached += 1;
	// pthread_mutex_unlock(&philo->data->mute_data);
	// pthread_mutex_unlock(&philo->mute_fork);
	// pthread_mutex_unlock(philo->mute_fork2);
	if (is_somone_dead_or_food_max_reached(data))
		return (1);
	return (0);
}

static int	have_a_nice_sleep(t_data *data)
{
	if (is_somone_dead_or_food_max_reached(data))
		return (1);
	data->timestamp = ft_get_timestamp(data->start_time);
	if (data->timestamp == -1)
		return (1);
	safe_print(data, "is sleeping");
	usleep(data->time_to_sleep * 1000);
	if (is_somone_dead_or_food_max_reached(data))
		return (1);
	return (0);
}

void	*routine(void	*arg)
{
	t_data	*data;

	data = arg;
	while (1)
	{
		if (data->hungry)
		{
			if (is_somone_dead_or_food_max_reached(data))
				return (NULL);
			if (take_forks(data))
				return (NULL);
			if (eat(data))
				return (NULL);
		}
		data->hungry = 1;
		if (have_a_nice_sleep(data))
			return (NULL);
		data->timestamp = ft_get_timestamp(data->start_time);
		if (data->timestamp == -1)
			return (NULL);
		safe_print(data, "is thinking");
		usleep(1000);
	}
	return (NULL);
}
