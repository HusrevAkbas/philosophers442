/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huakbas <huakbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:52:30 by husrevakbas       #+#    #+#             */
/*   Updated: 2025/04/02 14:23:25 by huakbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_data	*init_data(char **args)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return ((t_data *) ft_get_or_set_errors("Malloc failed in init_data function (1)"));
	data->philo_count = ft_atoi_safe(args[1]);
	data->time_to_die = ft_atoi_safe(args[2]);
	data->time_to_eat = ft_atoi_safe(args[3]);
	data->time_to_sleep = ft_atoi_safe(args[4]);
	if (args[5])
		data->food_max  = ft_atoi_safe(args[5]);
	else
		data->food_max = 0;
	data->start_time = ft_now();
	data->who_is_dead = malloc(sizeof(int));
	if (!data->who_is_dead)
		return ((t_data *) ft_free_many(data, NULL, NULL,
				"Malloc failed in init_data function (2)"));
	*data->who_is_dead = 0;
	data->food_max_reached = malloc(sizeof(int));
	if (!data->food_max_reached)
		return ((t_data *) ft_free_many(data, data->who_is_dead, NULL,
				"Malloc failed in init_data function (3)"));
	*data->food_max_reached = 0;
	pthread_mutex_init(&data->muter, NULL);
	return (data);
}

void	go_bath(t_philo **philos, t_data *data)
{
	int	i;

	if (data)
	{
		free (data->food_max_reached);
		free (data->who_is_dead);
		free (data);
		data = NULL;
	}
	if (!philos)
	return ;
	i = 0;
	while (philos[i])
	{
		free (philos[i]);
		i++;
	}
	free (philos);
}

t_philo	**init_philos(t_data *data)
{
	t_philo	**philos;
	int		i;

	philos = malloc(sizeof(t_philo *) * (data->philo_count + 1));
	if (!philos)
	{
		ft_get_or_set_errors("Malloc failed in init_philo function (1)");
		return (NULL);
	}
	memset(philos, 0, sizeof(t_philo *) * (data->philo_count + 1));
	i = 0;
	while (i < data->philo_count)
	{
		philos[i] = malloc(sizeof(t_philo));
		if (!philos[i])
		{
			ft_get_or_set_errors("Malloc failed in init_philo function (2)");
			go_bath(philos, data);
			return (NULL);
		}
		memset(philos[i], 0, sizeof(t_philo));
		if (i % 2 == 0)
			philos[i]->hungry = 1;
		philos[i]->name = i + 1;
		pthread_mutex_init(&philos[i]->mute_fork, NULL);
		philos[i]->data = data;
		i++;
	}
	i = 0;
	while (i < data->philo_count)
	{
		if (i == data->philo_count - 1)
		{
			if (i == 0)
			philos[i]->mute_fork2 = NULL;
			else
			philos[i]->mute_fork2 = &philos[0]->mute_fork;
			philos[i]->hungry = 0;
		}
		else
		{
			philos[i]->mute_fork2 = &philos[i + 1]->mute_fork;
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
	i = 0;
	while (philo[i])
	{
		time_since_last_meal = ft_get_timestamp(philo[i]->last_meal);
		if (time_since_last_meal > philo[i]->data->time_to_die)
		{
			*philo[i]->data->who_is_dead = philo[i]->name;
			printf("%5i %3d died\n", ft_get_timestamp(philo[i]->data->start_time), philo[i]->name);
			return (NULL);
		}
		i++;
	}
	return (NULL);
}
void	*routine(void	*arg)
{
	t_philo	*philo;
	int		timestamp;

	philo = arg;
	philo->last_meal = ft_now();
	while (1)
	{
		while (philo->hungry && !philo->mute_fork2)
		{
			if (*philo->data->who_is_dead || philo->data->philo_count == *philo->data->food_max_reached)
				return (NULL);
		}
		if (philo->hungry)
		{
			if (*philo->data->who_is_dead || philo->data->philo_count == *philo->data->food_max_reached)
				return (NULL);
			pthread_mutex_lock(&philo->mute_fork);
			pthread_mutex_lock(philo->mute_fork2);
			if (*philo->data->who_is_dead || philo->data->philo_count == *philo->data->food_max_reached)
			{
				pthread_mutex_unlock(&philo->mute_fork);
				pthread_mutex_unlock(philo->mute_fork2);
				return (NULL);
			}
			philo->food_counter++;
			if (philo->food_counter == philo->data->food_max)
			{
				pthread_mutex_lock(&philo->data->muter);
				*philo->data->food_max_reached += 1;
				pthread_mutex_unlock(&philo->data->muter);
			}
			timestamp = ft_get_timestamp(philo->data->start_time);
			printf("%5i %3d has taken a fork\n", timestamp, philo->name);
			printf("%5i %3d is eating(%d)\n", timestamp, philo->name, philo->food_counter);
			philo->last_meal = ft_now();
			usleep(philo->data->time_to_eat * 1000);
			pthread_mutex_unlock(&philo->mute_fork);
			pthread_mutex_unlock(philo->mute_fork2);
		}
		if (*philo->data->who_is_dead || philo->data->philo_count == *philo->data->food_max_reached)
			return (NULL);
		timestamp = ft_get_timestamp(philo->data->start_time);
		printf("%5i %3d is sleeping\n", timestamp, philo->name);
		usleep(philo->data->time_to_sleep * 1000);
		philo->hungry = 1;
		if (*philo->data->who_is_dead || philo->data->philo_count == *philo->data->food_max_reached)
			return (NULL);
		timestamp = ft_get_timestamp(philo->data->start_time);
		printf("%5i %3d is thinking\n", timestamp, philo->name);
		fflush(stdout); // WILL BE REMOVED
	}
	return (NULL);
}

void	delete_data(t_data *data)
{
	free(data->food_max_reached);
	free(data->who_is_dead);
	pthread_mutex_destroy(&data->muter);
	free(data);
}

int	main(int argc, char *argv[])
{
	int			i;
	t_philo		**philos;
	t_data		*data;

	if (argc < 5 || argc > 6)
		return (printf("Error: %s", WRONG_ARGUMENT_COUNT));
	i = 1;
	ft_check_args(argv);
	if (ft_get_or_set_errors(NULL))
		return (printf(INVALID_ARGUMENT));
	data = init_data(argv);
	if (!data)
		return (printf(MALLOC_FAIL));
	philos = init_philos(data);
	if (!philos)
		return (printf(MALLOC_FAIL));
	i = 0;
	while (philos[i])
	{
		pthread_create(&philos[i]->thread, NULL, &routine, philos[i]);
		i++;
	}
	while (*philos[0]->data->who_is_dead == 0
		&& *philos[0]->data->food_max_reached < philos[0]->data->philo_count)
	{
		am_i_dead(philos);
	}
	i = 0;
	while (philos[i])
	{
		pthread_join(philos[i]->thread, NULL);
		i++;
	}
	go_bath(philos, data);
	return (0);
}
