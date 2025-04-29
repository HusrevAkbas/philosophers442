/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huakbas <huakbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:52:30 by husrevakbas       #+#    #+#             */
/*   Updated: 2025/04/29 16:45:27 by huakbas          ###   ########.fr       */
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

t_data	*init_data(char **args)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return ((t_data *) ft_get_or_set_errors(
				"Malloc failed in init_data function (1)"));
	set_args(data, args);
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
		free(data->food_max_reached);
		free(data->who_is_dead);
		free(data);
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

t_philo	**create_philos(t_philo **philos, t_data *data)
{
	int	i;

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
	return (philos);
}

void	set_second_forks(t_philo **philos, t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		if (i == data->philo_count - 1)
		{
			if (data->philo_count == 1)
				philos[i]->mute_fork2 = NULL;
			else
				philos[i]->mute_fork2 = &philos[0]->mute_fork;
			philos[i]->hungry = 0;
		}
		else
			philos[i]->mute_fork2 = &philos[i + 1]->mute_fork;
		i++;
	}
}

t_philo	**init_philos(t_data *data)
{
	t_philo	**philos;

	philos = malloc(sizeof(t_philo *) * (data->philo_count + 1));
	if (!philos)
	{
		ft_get_or_set_errors("Malloc failed in init_philo function (1)");
		return (NULL);
	}
	memset(philos, 0, sizeof(t_philo *) * (data->philo_count + 1));
	philos = create_philos(philos, data);
	set_second_forks(philos, data);
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
		pthread_mutex_lock(&philo[i]->data->muter);
		if (time_since_last_meal > philo[i]->data->time_to_die)
		{
			*philo[i]->data->who_is_dead = philo[i]->name;
			printf("%5i %3d died\n", ft_get_timestamp(philo[i]->data->start_time),
				philo[i]->name);
			pthread_mutex_unlock(&philo[i]->data->muter);
			return (NULL);
		}
		pthread_mutex_unlock(&philo[i]->data->muter);
		i++;
	}
	return (NULL);
}

int	is_somone_dead_or_food_max_reached(t_data *data)
{
	pthread_mutex_lock(&data->muter);
	if (*data->who_is_dead || data->philo_count	== *data->food_max_reached)
	{
		pthread_mutex_unlock(&data->muter);
		return (1);
	}
	pthread_mutex_unlock(&data->muter);
	return (0);
}

int	take_forks(t_philo *philo)
{
	int	timestamp;

	pthread_mutex_lock(&philo->mute_fork);
	pthread_mutex_lock(&philo->data->muter);
	timestamp = ft_get_timestamp(philo->data->start_time);
	printf("%5i %3d has taken a fork 1\n", timestamp, philo->name);
	pthread_mutex_unlock(&philo->data->muter);
	if (philo->mute_fork2)
	{
		pthread_mutex_lock(&philo->data->muter);
		timestamp = ft_get_timestamp(philo->data->start_time);
		printf("%5i %3d has taken a fork 2\n", timestamp, philo->name);
		pthread_mutex_unlock(&philo->data->muter);
		pthread_mutex_lock(philo->mute_fork2);
	}
	else
	{
		while (!is_somone_dead_or_food_max_reached(philo->data));
		pthread_mutex_unlock(&philo->mute_fork);
		return (1);
	}
	return (0);
}

int	eat(t_philo *philo)
{
	if (is_somone_dead_or_food_max_reached(philo->data))
	{
		pthread_mutex_unlock(&philo->mute_fork);
		pthread_mutex_unlock(philo->mute_fork2);
		return (1);
	}
	philo->food_counter++;
	pthread_mutex_lock(&philo->data->muter);
	if (philo->food_counter == philo->data->food_max)
		*philo->data->food_max_reached += 1;
	philo->sleeptime = philo->data->time_to_eat * 1000;
	philo->timestamp = ft_get_timestamp(philo->data->start_time);
	pthread_mutex_unlock(&philo->data->muter);
	printf("%5i %3d is eating(%d)\n",
		philo->timestamp, philo->name, philo->food_counter);
	philo->last_meal = ft_now();
	usleep(philo->sleeptime);
	pthread_mutex_unlock(&philo->mute_fork);
	pthread_mutex_unlock(philo->mute_fork2);
	return (0);
}
void	*routine(void	*arg)
{
	t_philo	*philo;

	philo = arg;
	while (1)
	{
		if (philo->hungry)
		{
			if (take_forks(philo))
				return (NULL);
			if (eat(philo))
				return (NULL);
		}
		if (is_somone_dead_or_food_max_reached(philo->data))
			return (NULL);
		pthread_mutex_lock(&philo->data->muter);
		philo->timestamp = ft_get_timestamp(philo->data->start_time);
		philo->sleeptime = philo->data->time_to_sleep * 1000;
		pthread_mutex_unlock(&philo->data->muter);
		printf("%5i %3d is sleeping\n", philo->timestamp, philo->name);
		usleep(philo->sleeptime);
		philo->hungry = 1;
		if (is_somone_dead_or_food_max_reached(philo->data))
			return (NULL);
		pthread_mutex_lock(&philo->data->muter);
		philo->timestamp = ft_get_timestamp(philo->data->start_time);
		pthread_mutex_unlock(&philo->data->muter);
		printf("%5i %3d is thinking\n", philo->timestamp, philo->name);
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
	t_philo		**philos;
	t_data		*data;
	int			i;

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
		philos[i]->last_meal = ft_now();
		pthread_create(&philos[i]->thread, NULL, &routine, philos[i]);
		i++;
	}
	while (!is_somone_dead_or_food_max_reached(data))
		am_i_dead(philos);
	i = 0;
	while (philos[i])
		pthread_join(philos[i++]->thread, NULL);
	go_bath(philos, data);
	return (0);
}
