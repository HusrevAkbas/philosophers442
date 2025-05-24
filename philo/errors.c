/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huakbas <huakbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 11:39:12 by husrevakbas       #+#    #+#             */
/*   Updated: 2025/05/24 17:12:52 by huakbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

char	*ft_get_or_set_errors(char *error)
{
	static char	*err;

	if (!error)
		return (err);
	err = error;
	return (NULL);
}

int	unlock_mutex_return_one(pthread_mutex_t *mutex1, pthread_mutex_t *mutex2)
{
	if (mutex1)
		pthread_mutex_unlock(mutex1);
	if (mutex2)
		pthread_mutex_unlock(mutex2);
	return (1);
}

void	synch(t_philo *philo)
{
	while (ft_now() < philo->start_time)
		;
	philo->timestamp = ft_get_timestamp(philo->data->start_time);
	if (philo->name % 2 == 1)
	{
		usleep(350);
		safe_print(philo, "is thinking");
		sleep_disorder(philo->data, philo->time_to_eat);
	}
	if (philo->name % 2 == 1 && philo->name == philo->data->philo_count)
		sleep_disorder(philo->data, philo->time_to_eat / 2);
}
