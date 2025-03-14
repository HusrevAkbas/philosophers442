/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husrevakbas <husrevakbas@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:52:30 by husrevakbas       #+#    #+#             */
/*   Updated: 2025/03/14 23:06:04 by husrevakbas      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_philo	**init_philos(char **arg)
{
	t_philo	**philos;
	t_philo	arg_holder;
	int		i;
	int		*who_is_dead;
	pthread_mutex_t	*mute_dead;
	
	arg_holder.philo_count = ft_atoi_safe(arg[1]);
	arg_holder.time_to_die = ft_atoi_safe(arg[2]);
	arg_holder.time_to_eat = ft_atoi_safe(arg[3]);
	arg_holder.time_to_sleep = ft_atoi_safe(arg[4]);
	if (arg[5])
		arg_holder.food_max  = ft_atoi_safe(arg[5]);
	philos = malloc(sizeof(t_philo *) * (arg_holder.philo_count + 1));
	if (!philos)
		return (NULL);
	memset(philos, 0, sizeof(t_philo *) * (arg_holder.philo_count + 1));
	who_is_dead = malloc(sizeof(int));
	*who_is_dead = 0;
	mute_dead = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(mute_dead, NULL);
	i = 0;
	while (i < arg_holder.philo_count)
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
		philos[i]->philo_count = arg_holder.philo_count;
		philos[i]->food_max = arg_holder.food_max;
		philos[i]->who_is_dead = who_is_dead;
		philos[i]->time_to_die =arg_holder.time_to_die;
		philos[i]->time_to_eat =arg_holder.time_to_eat;
		philos[i]->time_to_sleep =arg_holder.time_to_sleep;
		philos[i]->mute_dead = mute_dead;
		arg_holder.mute_fork = malloc(sizeof(pthread_mutex_t)); //check malloc fail
		pthread_mutex_init(arg_holder.mute_fork, NULL);
		arg_holder.thread = malloc(sizeof(pthread_t)); //check malloc fail
		philos[i]->mute_fork = arg_holder.mute_fork;
		philos[i]->thread = arg_holder.thread;
		gettimeofday(&arg_holder.start_time, NULL); // check error
		philos[i]->start_time.tv_sec = arg_holder.start_time.tv_sec;
		philos[i]->start_time.tv_usec = arg_holder.start_time.tv_usec;
		philos[i]->last_meal.tv_sec = arg_holder.start_time.tv_sec;
		philos[i]->last_meal.tv_usec = arg_holder.start_time.tv_usec;
		i++;
	}
	i = 0;
	while (i < arg_holder.philo_count)
	{
		if (i == arg_holder.philo_count - 1)
		{
			philos[i]->fork2 = &philos[0]->fork;
			if (i == 0)
				philos[i]->mute_fork2 = NULL;
			else
				philos[i]->mute_fork2 = philos[0]->mute_fork;
			philos[i]->hungry = 0;
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
	t_philo	*philo;
	int		time_since_last_meal;

	philo = arg;
	while (1)
	{
		time_since_last_meal = ft_get_timestamp(philo->last_meal);
		if (philo->last_meal.tv_sec && time_since_last_meal > philo->time_to_die)
		{
			pthread_mutex_lock(philo->mute_dead);
			*philo->who_is_dead = philo->name;
			printf("%5i %3d died\n", ft_get_timestamp(philo->start_time), philo->name);
			return (NULL);
		}
	}
	return (NULL);
}
void	*routine(void	*arg)
{
	pthread_t	th;
	t_philo	*philo;
	int		timestamp;

	philo = arg;
	th = pthread_create(&th, NULL, &am_i_dead, philo);
	
	pthread_detach(th);
	while (1)
	{
		while (philo->hungry && !philo->mute_fork2)
		{
			if (*philo->who_is_dead)
				return (NULL);
		}
		if (philo->hungry)
		{
			pthread_mutex_lock(philo->mute_fork);
			pthread_mutex_lock(philo->mute_fork2);
			if (*philo->who_is_dead)
				return (NULL);
			timestamp = ft_get_timestamp(philo->start_time);
			printf("%5i %3d has taken a fork\n", timestamp, philo->name);
			philo->last_meal.tv_sec = 0;
			printf("%5i %3d is eating\n", timestamp, philo->name);
			usleep(philo->time_to_eat * 1000);
			ft_update_last_meal(&philo->last_meal);
			pthread_mutex_unlock(philo->mute_fork);
			pthread_mutex_unlock(philo->mute_fork2);
		}
		if (*philo->who_is_dead)
			return (NULL);
		timestamp = ft_get_timestamp(philo->start_time);
		printf("%5i %3d is sleeping\n", timestamp, philo->name);
		usleep(philo->time_to_sleep * 1000);
		philo->hungry = 1;
		if (*philo->who_is_dead)
			return (NULL);
		timestamp = ft_get_timestamp(philo->start_time);
		printf("%5i %3d is thinking\n", timestamp, philo->name);
		if (philo->time_to_die - philo->time_to_sleep - 5 > 0)
			usleep((philo->time_to_die - philo->time_to_sleep - 5) * 1000);
		fflush(stdout); // WILL BE REMOVED
		//check if anyone dies
	}
	return (NULL);
}

int	main(int argc, char *argv[])
{
	int				i;
	t_philo			**philos;

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
	i = 0;
	while (philos[i])
	{
		pthread_join(*philos[i]->thread, NULL);
		i++;
	}
	return (0);
}
