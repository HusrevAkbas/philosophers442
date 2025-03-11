/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husrevakbas <husrevakbas@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:52:30 by husrevakbas       #+#    #+#             */
/*   Updated: 2025/03/11 23:04:48 by husrevakbas      ###   ########.fr       */
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

t_philo	**init_philos(char *arg, pthread_mutex_t *mutex)
{
	t_philo	**philos;
	int		count;
	int		i;
	
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
		philos[i]->mutex = mutex;
		i++;
	}
	return (philos);
}

void	*routine(void	*arg)
{
	t_philo	**philos;
	
	philos = arg;
	pthread_mutex_lock(philos[0]->mutex);
	philos[1]->fork++;
	printf("Eat fork: %d\n", philos[1]->fork);
	sleep(1);
	pthread_mutex_unlock(philos[0]->mutex);
	philos[1]->fork++;
	printf("sleep fork %d\n", philos[1]->fork);
	sleep(1);
	philos[1]->fork++;
	printf("think fork: %d\n", philos[1]->fork);
	sleep(1);
	return (NULL);
}

int	main(int argc, char *argv[])
{
	int				i;
	t_philo			**philos;
	pthread_mutex_t	mutex;
	pthread_t		th1;
	pthread_t		th2;

	if (argc < 5 || argc > 6)
		return (printf("Error: %s", WRONG_ARGUMENT_COUNT));
	i = 1;
	check_args(argv);
	if (ft_get_or_set_errors(NULL))
	return (printf(INVALID_ARGUMENT));
	pthread_mutex_init(&mutex, NULL);
	philos = init_philos(argv[1], &mutex);
	if (!philos)
		return (printf(MALLOC_FAIL));
	i = 0;
	while (philos[i])
	{
		printf("philo %d : %d\n", philos[i]->name, philos[i]->fork);
		i++;
	}
	pthread_create(&th1, NULL, &routine, philos);
	pthread_create(&th2, NULL, &routine, philos);
	pthread_join(th1, NULL);
	pthread_join(th2, NULL);
	return (0);
}
