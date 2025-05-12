/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huakbas <huakbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:50:19 by huakbas           #+#    #+#             */
/*   Updated: 2025/05/12 17:44:02 by huakbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	take_forks(t_data *data)
{
	sem_wait(data->semaphore);
	if (data->return_code)
	{
		sem_post(data->semaphore);
		return (data->return_code);
	}
	if (safe_print(data, "is taking a fork 1"))
	{
		sem_post(data->semaphore);
		return (data->return_code);
	}
	sem_wait(data->semaphore);
	if (data->return_code)
	{
		sem_post(data->semaphore);
		sem_post(data->semaphore);
		return (data->return_code);
	}
	if (safe_print(data, "is taking a fork 2"))
	{
		sem_post(data->semaphore);
		sem_post(data->semaphore);
		return (data->return_code);
	}
	return (0);
}

static int	eat(t_data *data)
{
	data->food_counter++;
	if (safe_print(data, "is eating"))
		return (data->return_code = 3, 3);
	if (data->food_counter == data->food_max)
	{
		sem_post(data->semaphore);
		sem_post(data->semaphore);
		return (data->return_code = 2, 2);
	}
	data->last_meal = ft_now();
	if (data->last_meal == -1)
	{
		sem_post(data->semaphore);
		sem_post(data->semaphore);
		return (data->return_code = 3, 3);
	}
	usleep(data->time_to_eat * 1000);
	sem_post(data->semaphore);
	sem_post(data->semaphore);
	if (data->return_code)
		return (data->return_code);
	if (data->food_counter == data->food_max)
		return (data->return_code = 2, 2);
	return (0);
}

static int	have_a_nice_sleep(t_data *data)
{
	if (safe_print(data, "is sleeping"))
		return (3);
	usleep(data->time_to_sleep * 1000);
	if (data->return_code)
		return (1);
	return (0);
}

void	*routine(void *arg)
{
	t_data	*data;

	data = arg;
	while (1)
	{
		if (data->return_code)
			return (&data->return_code);
		if (take_forks(data))
			return (&data->return_code);
		eat(data);
		if (data->return_code)
			return (&data->return_code);
		have_a_nice_sleep(data);
		if (data->return_code)
			return (&data->return_code);
		pthread_mutex_lock(&data->mute_data);
		data->timestamp = ft_get_timestamp(data->start_time);
		pthread_mutex_unlock(&data->mute_data);
		if (data->timestamp == -1)
			return (data->return_code = 3, &data->return_code);
		if (safe_print(data, "is thinking"))
			return (&data->return_code);
		usleep(1000);
	}
	return (&data->return_code);
}
