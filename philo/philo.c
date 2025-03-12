/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husrevakbas <husrevakbas@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:52:30 by husrevakbas       #+#    #+#             */
/*   Updated: 2025/03/12 15:53:32 by husrevakbas      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	check_args(char **argv)
{
	int	i;
	int	a;

	i = 1;
	while (argv[i])
	{
		a = ft_atoi_safe(argv[i]);
		if (a == 0 && ft_get_or_set_errors(NULL))
			printf("Argument %d: %s", i, ft_get_or_set_errors(NULL));
		else if (a < 0 || a == 0)
		{
			ft_get_or_set_errors(NEGATIVE_NUMBER);
			printf("Argument %d: %s", i, NEGATIVE_NUMBER);
		}
		i++;
	}
}

t_philo	**init_philos(char *arg)
{
	t_philo	**philos;
	int		count;
	int		i;
	pthread_mutex_t	*mutex;
	pthread_t	*thread;
	t_timeval	tv;
	
	count = ft_atoi_safe(arg);
	philos = malloc(sizeof(t_philo *) * (count + 1));
	if (!philos)
		return (NULL);
	memset(philos, 0, sizeof(t_philo *) * (count + 1));
	i = 0;
	while (i < count)
	{
		philos[i] = malloc(sizeof(t_philo));
		if (!philos[i])
		{	// CLEAR BEFORE RETURN
			return (NULL);
		}
		philos[i]->name = i + 1;
		mutex = malloc(sizeof(pthread_mutex_t)); //check malloc fail
		pthread_mutex_init(mutex, NULL);
		thread = malloc(sizeof(pthread_t)); //check malloc fail
		philos[i]->mutex = mutex;
		philos[i]->thread = thread;
		gettimeofday(&tv, NULL);
		philos[i]->start_time.tv_sec = tv.tv_sec;
		philos[i]->start_time.tv_usec = tv.tv_usec;
		i++;
	}
	i = 0;
	while (i < count)
	{
		if (i == count - 1)
		{
			philos[i]->fork2 = &philos[0]->fork;
			philos[i]->mutex2 = philos[0]->mutex;
		}
		else
		{
			philos[i]->fork2 = &philos[i + 1]->fork;
			philos[i]->mutex2 = philos[i + 1]->mutex;
		}
		i++;
	}
	return (philos);
}

void	*routine(void	*arg)
{
	t_philo	*philo;
	int		timestamp; //create a function to calculate timestamp
	t_timeval	tv;
	
	philo = arg;
	while (1)
	{
		printf("%li %d read start time sec\n", philo->start_time.tv_sec, philo->name);
		printf("%li %d read start time usec\n", philo->start_time.tv_usec, philo->name);
		sleep(1);
		pthread_mutex_lock(philo->mutex);
		pthread_mutex_lock(philo->mutex2);
		gettimeofday(&tv, NULL);
		timestamp = tv.tv_usec - philo->start_time.tv_usec;
		printf("%i %d has taken a fork\n", timestamp / 1000, philo->name);
		printf("philo %d is eating\n", philo->name);
		usleep(500 * 1000);
		pthread_mutex_unlock(philo->mutex);
		pthread_mutex_unlock(philo->mutex2);
		gettimeofday(&tv, NULL);
		timestamp = tv.tv_usec - philo->start_time.tv_usec;
		printf("%i %d is sleeping\n", timestamp / 1000, philo->name);
		usleep(500 * 1000);
		gettimeofday(&tv, NULL);
		timestamp = tv.tv_usec - philo->start_time.tv_usec;
		printf("%i %d is thinking \n", timestamp / 1000, philo->name);
		usleep(500 * 1000);
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
	check_args(argv);
	if (ft_get_or_set_errors(NULL))
		return (printf(INVALID_ARGUMENT));
	philos = init_philos(argv[1]);
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
		free(philos[i]->thread);
		i++;
	}
	return (0);
}
