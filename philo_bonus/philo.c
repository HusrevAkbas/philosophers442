/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huakbas <huakbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:52:30 by husrevakbas       #+#    #+#             */
/*   Updated: 2025/05/07 18:55:01 by huakbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	set_args(t_data *data, char **args)
{
	data->philo_count = ft_atoi_safe(args[1]);
	data->time_to_die = ft_atoi_safe(args[2]);
	data->time_to_eat = ft_atoi_safe(args[3]);
	data->time_to_sleep = ft_atoi_safe(args[4]);
	if (args[5])
		data->food_max = ft_atoi_safe(args[5]);
	else
		data->food_max = 0;
}

int	*init_data(t_data *data, char **args)
{
	set_args(data, args);
	data->start_time = ft_now();
	data->who_is_dead = 0;
	data->food_max_reached = 0;
	// pthread_mutex_init(&data->mute_data, NULL);
	// pthread_mutex_init(&data->mute_print, NULL);
	return (data);
}

void	*am_i_dead(void	*arg)
{
	t_philo	**philos;
	int		time_since_last_meal;
	int		i;

	philos = arg;
	i = 0;
	while (philos[i])
	{
		usleep(1000 / philos[0]->data->philo_count);
		//pthread_mutex_lock(&philos[i]->data->mute_data);
		time_since_last_meal = ft_get_timestamp(philos[i]->last_meal);
		//pthread_mutex_unlock(&philos[i]->data->mute_data);
		if (time_since_last_meal == -1)
			return (NULL);
		if (time_since_last_meal > philos[i]->time_to_die)
		{
			//pthread_mutex_lock(&philos[i]->data->mute_data);
			philos[i]->data->who_is_dead = philos[i]->name;
			//pthread_mutex_unlock(&philos[i]->data->mute_data);
			safe_print(philos[i], "died");
			return (NULL);
		}
		i++;
	}
	return (NULL);
}

int	start_threads(t_philo **philos)
{
	int	i;
	int	j;

	i = 0;
	while (philos[i])
	{
		philos[i]->last_meal = ft_now();
		if (pthread_create(&philos[i]->thread, NULL, &routine, philos[i]))
		{
			ft_get_or_set_errors(ERROR_CREATING_THREAD);
			break ;
		}
		i++;
	}
	if (ft_get_or_set_errors(NULL))
	{
		j = 0;
		while (j < i)
			pthread_join(philos[j++]->thread, NULL);
		go_to_bath(philos, philos[0]->data);
		return (1);
	}
	return (0);
}

int	main(int argc, char *argv[])
{
	t_philo		philo;
	t_data		data;
	int			i;

	if (argc < 5 || argc > 6)
		return (printf("Error: %s", WRONG_ARGUMENT_COUNT));
	ft_check_args(argv);
	if (ft_get_or_set_errors(NULL))
		return (printf(INVALID_ARGUMENT));
	init_data(&data, argv);
	init_philos(&philo, &data);
	// if (start_threads(philo)) // will be replaced with fork functions
	// 	return (printf(ERROR_CREATING_THREAD));
	// while (!is_somone_dead_or_food_max_reached(data)) will be done in child process
	// 	am_i_dead(philo);
	// i = 0;
	// while (philo[i])	will be waited for child processes
	// 	pthread_join(philo[i++]->thread, NULL);
	// go_to_bath(philo, data);	// no more allocated memory. will be handled in waitpid
	return (0);
}
