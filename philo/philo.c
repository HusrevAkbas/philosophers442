/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huakbas <huakbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:52:30 by husrevakbas       #+#    #+#             */
/*   Updated: 2025/04/30 15:00:54 by huakbas          ###   ########.fr       */
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
	pthread_mutex_init(&data->mute_data, NULL);
	pthread_mutex_init(&data->mute_print, NULL);
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
		pthread_mutex_destroy(&philos[i]->mute_fork);
		pthread_mutex_destroy(&philos[i]->mute_philo);
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
		pthread_mutex_init(&philos[i]->mute_philo, NULL);
		philos[i]->data = data;
		philos[i]->time_to_die = data->time_to_die;
		philos[i]->time_to_eat = data->time_to_eat;
		philos[i]->time_to_sleep = data->time_to_sleep;
		philos[i]->start_time = data->start_time;
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
	t_philo	**philos;
	int		time_since_last_meal;
	int		i;
	int		philo_count;

	philos = arg;
	i = 0;
	philo_count = philos[0]->data->philo_count;
	while (philos[i])
	{
		usleep(100 / philo_count);
		pthread_mutex_lock(&philos[i]->mute_philo);
		time_since_last_meal = ft_get_timestamp(philos[i]->last_meal);
		if (time_since_last_meal > philos[i]->time_to_die)
		{
			pthread_mutex_lock(&philos[i]->data->mute_data);
			*philos[i]->data->who_is_dead = philos[i]->name;
			pthread_mutex_unlock(&philos[i]->data->mute_data);
			safe_print(philos[i], "died");
			pthread_mutex_unlock(&philos[i]->mute_philo);
			return (NULL);
		}
		pthread_mutex_unlock(&philos[i]->mute_philo);
		i++;
	}
	return (NULL);
}

int	is_somone_dead_or_food_max_reached(t_data *data)
{
	pthread_mutex_lock(&data->mute_data);
	if (*data->who_is_dead || data->philo_count	== *data->food_max_reached)
	{
		pthread_mutex_unlock(&data->mute_data);
		return (1);
	}
	pthread_mutex_unlock(&data->mute_data);
	usleep(50);
	return (0);
}

int	take_forks(t_philo *philo)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	if (philo->mute_fork2 > &philo->mute_fork)
	{
		first = philo->mute_fork2;
		second = &philo->mute_fork;
	}
	else
	{
		first = &philo->mute_fork;
		second = philo->mute_fork2;
	}
	safe_print(philo, "has taken a fork 1");
	pthread_mutex_lock(first);
	if (philo->mute_fork2)
	{
		safe_print(philo, "has taken a fork 2");
		pthread_mutex_lock(second);
	}
	else
	{
		pthread_mutex_unlock(&philo->mute_philo);
		pthread_mutex_unlock(first);
		while (!is_somone_dead_or_food_max_reached(philo->data));
		return (1);
	}
	return (0);
}

int	eat(t_philo *philo)
{
	philo->last_meal = ft_now();
	safe_print(philo, "is eating");
	philo->food_counter++;
	pthread_mutex_lock(&philo->data->mute_data);
	if (philo->food_counter == philo->data->food_max)
		*philo->data->food_max_reached += 1;
	pthread_mutex_unlock(&philo->data->mute_data);
	pthread_mutex_unlock(&philo->mute_fork);
	pthread_mutex_unlock(philo->mute_fork2);
	return (0);
}

int	have_a_nice_sleep(t_philo *philo)
{
	if (is_somone_dead_or_food_max_reached(philo->data))
		return (1);
	pthread_mutex_lock(&philo->mute_philo);
	pthread_mutex_lock(&philo->data->mute_data);
	philo->timestamp = ft_get_timestamp(philo->data->start_time);
	pthread_mutex_unlock(&philo->data->mute_data);
	safe_print(philo, "is sleeping");
	pthread_mutex_unlock(&philo->mute_philo);
	usleep(philo->time_to_sleep * 1000);
	if (is_somone_dead_or_food_max_reached(philo->data))
		return (1);
	return (0);
}

void	*routine(void	*arg)
{
	t_philo	*philo;

	philo = arg;
	while (1)
	{
		pthread_mutex_lock(&philo->mute_philo);
		if (philo->hungry)
		{
			if (take_forks(philo))
				return (NULL);
			if (eat(philo))
				return (NULL);
		}
		philo->hungry = 1;
		pthread_mutex_unlock(&philo->mute_philo);
		if (have_a_nice_sleep(philo))
			return (NULL);
		pthread_mutex_lock(&philo->mute_philo);
		pthread_mutex_lock(&philo->data->mute_data);
		philo->timestamp = ft_get_timestamp(philo->data->start_time);
		pthread_mutex_unlock(&philo->data->mute_data);
		safe_print(philo, "is thinking");
		pthread_mutex_unlock(&philo->mute_philo);
	}
	return (NULL);
}

void	delete_data(t_data *data)
{
	free(data->food_max_reached);
	free(data->who_is_dead);
	pthread_mutex_destroy(&data->mute_data);
	pthread_mutex_destroy(&data->mute_print);
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
