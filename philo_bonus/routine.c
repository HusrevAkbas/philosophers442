/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huakbas <huakbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:50:19 by huakbas           #+#    #+#             */
/*   Updated: 2025/05/12 13:55:19 by huakbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	take_forks(t_data *data)
{
	sem_wait(data->semaphore);
	if (data->am_i_dead)
		return (1);
	if (safe_print(data, "is taking a fork 1"))
		return (3);
	sem_wait(data->semaphore);
	if (data->am_i_dead)
		return (1);
	if (safe_print(data, "is taking a fork 2"))
		return (3);
	return (0);
}

static int	eat(t_data *data)
{
	safe_print(data, "is eating");
	data->last_meal = ft_now();
	if (data->last_meal == -1)
		return (3);
	data->food_counter++;
	usleep(data->time_to_eat * 1000);
	sem_post(data->semaphore);
	sem_post(data->semaphore);
	if (data->am_i_dead)
		return (1);
	if (data->name == 1)
		printf("counter: %i food max: %i\n", data->food_counter, data->food_max);
	if (data->food_counter == data->food_max)
		return (2);
	return (0);
}

static int	have_a_nice_sleep(t_data *data)
{
	if (safe_print(data, "is sleeping"))
		return (3);
	usleep(data->time_to_sleep * 1000);
	if (data->am_i_dead)
		return (1);
	return (0);
}

int	routine(t_data *data)
{
	int	res;

	while (1)
	{
		if (data->am_i_dead)
			return (1);
		if (take_forks(data))
			return (1);
		res = eat(data);
		if (res)
			return (res);
		res = have_a_nice_sleep(data);
		if (res)
			return (res);
		data->timestamp = ft_get_timestamp(data->start_time);
		if (data->timestamp == -1)
			return (3);
		if (safe_print(data, "is thinking"))
			return (3);
		usleep(1000);
	}
	return (0);
}
