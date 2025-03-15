/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husrevakbas <husrevakbas@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:52:30 by husrevakbas       #+#    #+#             */
/*   Updated: 2025/03/15 22:54:30 by husrevakbas      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_philo	**init_philos(char **arg)
{
	t_philo	**philos;
	t_data	*data;
	int		i;
	int		*who_is_dead;
	
	data = malloc(sizeof(t_data));
	data->philo_count = ft_atoi_safe(arg[1]);
	data->time_to_die = ft_atoi_safe(arg[2]);
	data->time_to_eat = ft_atoi_safe(arg[3]);
	data->time_to_sleep = ft_atoi_safe(arg[4]);
	if (arg[5])
		data->food_max  = ft_atoi_safe(arg[5]);
	philos = malloc(sizeof(t_philo *) * (data->philo_count + 1));
	if (!philos)
		return (NULL);
	memset(philos, 0, sizeof(t_philo *) * (data->philo_count + 1));
	who_is_dead = malloc(sizeof(int)); // check fail
	*who_is_dead = 0;
	data->who_is_dead = who_is_dead;
	i = 0;
	while (i < data->philo_count)
	{
		philos[i] = malloc(sizeof(t_philo));
		if (!philos[i])
		{	// CLEAR BEFORE RETURN
			return (NULL);
		}
		memset(philos[i], 0, sizeof(t_philo));
		if (i % 2 == 0)
			philos[i]->hungry = 1;
		philos[i]->name = i + 1;
		data->muter = malloc(sizeof(pthread_mutex_t)); //check malloc fail
		pthread_mutex_init(data->muter, NULL);
		philos[i]->mute_fork = data->muter;
		data->thread = malloc(sizeof(pthread_t)); //check malloc fail
		philos[i]->thread = data->thread;
		gettimeofday(&data->start_time, NULL); // check error
		philos[i]->start_time.tv_sec = data->start_time.tv_sec;
		philos[i]->start_time.tv_usec = data->start_time.tv_usec;
		philos[i]->last_meal.tv_sec = data->start_time.tv_sec;
		philos[i]->last_meal.tv_usec = data->start_time.tv_usec;
		philos[i]->data = data;
		data->muter = NULL;
		data->thread = NULL;
		i++;
	}
	i = 0;
	while (i < data->philo_count)
	{
		if (i == data->philo_count - 1)
		{
			philos[i]->fork2 = &philos[0]->fork;
			if (i == 0)
				philos[i]->mute_fork2 = NULL;
			else
				philos[i]->mute_fork2 = philos[0]->mute_fork;
			philos[i]->hungry = 0;
			if (i % 2 == 1)
				philos[i]->sleepy = 1;
		}
		else
		{
			philos[i]->fork2 = &philos[i + 1]->fork;
			philos[i]->mute_fork2 = philos[i + 1]->mute_fork;
		}
		i++;
	}
	return (philos);
}
void	*am_i_dead(void	*arg)
{
	t_philo	**philo;
	int		time_since_last_meal;
	int		i;

	philo = arg;
	while (1)
	{
		i = 0;
		while (philo[i])
		{
			time_since_last_meal = ft_get_timestamp(philo[i]->last_meal);
			if (time_since_last_meal > philo[i]->data->time_to_die)
			{
				*philo[i]->data->who_is_dead = philo[i]->name;
				printf("%5i %3d died\n", ft_get_timestamp(philo[i]->start_time), philo[i]->name);
				return (NULL);
			}
			i++;
		}
	}
	return (NULL);
}
void	*routine(void	*arg)
{
	t_philo	*philo;
	int		timestamp;

	philo = arg;
	ft_update_last_meal(&philo->start_time);
	philo->last_meal.tv_sec =  philo->data->start_time.tv_sec;
	philo->last_meal.tv_usec = philo->data->start_time.tv_usec;
	while (1)
	{
		while (philo->hungry && !philo->mute_fork2)
		{
			if (*philo->data->who_is_dead)
			return (NULL);
		}
		if (philo->hungry)
		{
			if (*philo->data->who_is_dead)
				return (NULL);
			pthread_mutex_lock(philo->mute_fork);
			pthread_mutex_lock(philo->mute_fork2);
			if (*philo->data->who_is_dead)
				return (NULL);
			timestamp = ft_get_timestamp(philo->start_time);
			printf("%5i %3d has taken a fork\n", timestamp, philo->name);
			printf("%5i %3d is eating\n", timestamp, philo->name);
			ft_update_last_meal(&philo->last_meal);
			usleep(philo->data->time_to_eat * 1000);
			pthread_mutex_unlock(philo->mute_fork);
			pthread_mutex_unlock(philo->mute_fork2);
		}
		if (*philo->data->who_is_dead)
			return (NULL);
		timestamp = ft_get_timestamp(philo->start_time);
		printf("%5i %3d is sleeping\n", timestamp, philo->name);
		usleep(philo->data->time_to_sleep * 1000);
		philo->hungry = 1;
		if (*philo->data->who_is_dead)
			return (NULL);
		timestamp = ft_get_timestamp(philo->start_time);
		printf("%5i %3d is thinking\n", timestamp, philo->name);
		// if (!philo->sleepy && philo->data->time_to_die - philo->data->time_to_eat - philo->data->time_to_sleep - 30 > 0)
		// 	usleep((philo->data->time_to_die - philo->data->time_to_eat - philo->data->time_to_sleep - 30) * 1000);
		fflush(stdout); // WILL BE REMOVED
		//check if anyone dies
	}
	return (NULL);
}

int	main(int argc, char *argv[])
{
	int			i;
	t_philo		**philos;
	pthread_t	dead_check;

	if (argc < 5 || argc > 6)
		return (printf("Error: %s", WRONG_ARGUMENT_COUNT));
	i = 1;
	ft_check_args(argv);
	if (ft_get_or_set_errors(NULL))
		return (printf(INVALID_ARGUMENT));
	philos = init_philos(argv);
	if (!philos)
		return (printf(MALLOC_FAIL));
	i = 0;
	while (philos[i])
	{
		pthread_create(philos[i]->thread, NULL, &routine, philos[i]);
		i++;
	}
	pthread_create(&dead_check, NULL, &am_i_dead, philos);
	pthread_join(dead_check, NULL);
	i = 0;
	while (philos[i])
	{
		pthread_join(*philos[i]->thread, NULL);
		i++;
	}
	return (0);
}
