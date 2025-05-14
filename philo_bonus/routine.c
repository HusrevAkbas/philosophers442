/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huakbas <huakbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:50:19 by huakbas           #+#    #+#             */
/*   Updated: 2025/05/14 13:36:01 by huakbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	set_or_check_return_code(t_data *data, int num)
{
	sem_wait(data->sem_mute);
	if (num == 0)
	{
		if (data->return_code)
		{
			sem_post(data->sem_mute);
			return (data->return_code);
		}
		else
		{
			sem_post(data->sem_mute);
			return (0);
		}
	}
	else
	{
		data->return_code = num;
		sem_post(data->sem_mute);
		return (num);
	}
	sem_post(data->sem_mute);
	return (0);
}

static int	take_forks(t_data *data)
{
	sem_wait(data->sem_fork);
	if (set_or_check_return_code(data, 0))
	{
		sem_post(data->sem_fork);
		return (data->return_code);
	}
	if (safe_print(data, "is taking a fork 1"))
	{
		sem_post(data->sem_fork);
		return (data->return_code);
	}
	sem_wait(data->sem_fork);
	if (set_or_check_return_code(data, 0))
	{
		sem_post(data->sem_fork);
		sem_post(data->sem_fork);
		return (data->return_code);
	}
	if (safe_print(data, "is taking a fork 2"))
	{
		sem_post(data->sem_fork);
		sem_post(data->sem_fork);
		return (data->return_code);
	}
	return (0);
}

static int	eat(t_data *data)
{
	sem_wait(data->sem_mute);
	data->food_counter++;
	sem_post(data->sem_mute);
	if (safe_print(data, "is eating"))
		return (set_or_check_return_code(data, 3));
	sem_wait(data->sem_mute);
	data->last_meal = ft_now();
	if (data->last_meal == -1)
	{
		sem_post(data->sem_mute);
		sem_post(data->sem_fork);
		sem_post(data->sem_fork);
		return (set_or_check_return_code(data, 3));
	}
	sem_post(data->sem_mute);
	usleep(data->time_to_eat * 1000);
	sem_post(data->sem_fork);
	sem_post(data->sem_fork);
	if (data->food_counter == data->food_max + 2)
		return (set_or_check_return_code(data, 2));
	if (set_or_check_return_code(data, 0))
		return (data->return_code);
	return (0);
}

static int	have_a_nice_sleep(t_data *data)
{
	if (safe_print(data, "is sleeping"))
		return (3);
	usleep(data->time_to_sleep * 1000);
	sem_wait(data->sem_mute);
	if (data->return_code)
	{
		sem_post(data->sem_mute);
		return (1);
	}
	sem_post(data->sem_mute);
	return (0);
}

int	routine(t_data *data)
{
	while (1)
	{
		sem_wait(data->sem_mute);
		if (data->return_code)
			return (data->return_code);
		sem_post(data->sem_mute);
		if (take_forks(data))
			return (data->return_code);
		eat(data);
		if (data->return_code)
			return (data->return_code);
		have_a_nice_sleep(data);
		if (data->return_code)
			return (data->return_code);
		data->timestamp = ft_get_timestamp(data->start_time);
		if (data->timestamp == -1)
			return (data->return_code = 3, data->return_code);
		if (safe_print(data, "is thinking"))
			return (data->return_code);
		usleep(1000);
	}
	return (data->return_code);
}
