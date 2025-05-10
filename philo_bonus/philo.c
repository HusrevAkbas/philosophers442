/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huakbas <huakbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:52:30 by husrevakbas       #+#    #+#             */
/*   Updated: 2025/05/10 17:44:49 by huakbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// void	set_args(t_data *data, char **args)
// {
// 	data->philo_count = ft_atoi_safe(args[1]);
// 	data->time_to_die = ft_atoi_safe(args[2]);
// 	data->time_to_eat = ft_atoi_safe(args[3]);
// 	data->time_to_sleep = ft_atoi_safe(args[4]);
// 	if (args[5])
// 		data->food_max = ft_atoi_safe(args[5]);
// 	else
// 		data->food_max = 0;
// }

int	init_data(t_data *data, char **args)
{
	//set_args(data, args);
	data->philo_count = ft_atoi_safe(args[1]);
	data->time_to_die = ft_atoi_safe(args[2]);
	data->time_to_eat = ft_atoi_safe(args[3]);
	data->time_to_sleep = ft_atoi_safe(args[4]);
	if (args[5])
		data->food_max = ft_atoi_safe(args[5]);
	else
		data->food_max = 0;
	data->start_time = ft_now();
	data->who_is_dead = 0;
	data->food_max_reached = 0;
	// pthread_mutex_init(&data->mute_data, NULL);
	// pthread_mutex_init(&data->mute_print, NULL);
	return (0);
}

void	*am_i_dead(void	*arg)
{
	t_data	*data;
	int		time_since_last_meal;
	int		i;

	data = arg;
	i = 0;
	while (1)
	{
		usleep(1000);
		//pthread_mutex_lock(&philos[i]->data->mute_data);
		time_since_last_meal = ft_get_timestamp(data->last_meal);
		//pthread_mutex_unlock(&philos[i]->data->mute_data);
		if (time_since_last_meal == -1)
			return (NULL);
		if (time_since_last_meal > data->time_to_die)
		{
			//pthread_mutex_lock(&philos[i]->data->mute_data);
			data->who_is_dead = data->name;
			//pthread_mutex_unlock(&philos[i]->data->mute_data);
			safe_print(data, "died");
			return (NULL);
		}
		i++;
	}
	return (NULL);
}

// int	start_threads(t_philo *philos)
// {
// 	(void) philos;
	// int	i;
	// int	j;

	// i = 0;
	// while (philos[i])
	// {
	// 	philos[i]->last_meal = ft_now();
	// 	if (pthread_create(&philos[i]->thread, NULL, &routine, philos[i]))
	// 	{
	// 		ft_get_or_set_errors(ERROR_CREATING_THREAD);
	// 		break ;
	// 	}
	// 	i++;
	// }
	// if (ft_get_or_set_errors(NULL))
	// {
	// 	j = 0;
	// 	while (j < i)
	// 		pthread_join(philos[j++]->thread, NULL);
	// 	go_to_bath(philos, philos[0]->data);
	// 	return (1);
	// }
// 	return (0);
// }

void	child(t_data data, int *pids)
{
	free (pids);
	sem_close(data.semaphore);
	data.semaphore = sem_open(SEM_NAME, O_RDWR);
	if (data.semaphore == SEM_FAILED)
	{
		perror("SEM FAILED");
		// printf("sem fail in child\n");
	}
	usleep(100 * data.name);
	//sem_unlink(SEM_NAME);
	//work on routine. if someone dies or full of meal exit.
	int j = 0;
	int val = -1;
	while (j < data.name)
	{
		sem_wait(data.semaphore);
		sem_wait(data.semaphore);
		sem_getvalue(data.semaphore, &val);
		printf("i am working in child with philo %i, semaphore value: %i\n", data.name, val);
		sleep(1);
		sem_post(data.semaphore);
		sem_post(data.semaphore);
		j++;
	}
	sem_close(data.semaphore);
	exit(data.name);
}

int	start_child_processes(t_data data, int *pids)
{
	unsigned int	i;
	
	i = 0;
	while (i < data.philo_count)
	{
		data.name = i + 1;
		data.last_meal = ft_now();
		pids[i] = fork();
		if (pids[i] == -1)
			return (i);
		if (pids[i] == 0) //call child process function
			child(data, pids);
		i++;
	}
	return (0);
}

int	main(int argc, char *argv[])
{
	t_data	data;
	int		i;
	pid_t	*pids;
	int		status;

	if (argc < 5 || argc > 6)
		return (printf("Error: %s", WRONG_ARGUMENT_COUNT));
	ft_check_args(argv);
	if (ft_get_or_set_errors(NULL))
		return (printf(INVALID_ARGUMENT));
	init_data(&data, argv);
	sem_unlink(SEM_NAME);
	data.semaphore = sem_open(SEM_NAME, O_CREAT | O_RDWR, 0777, (unsigned int) data.philo_count);
	if (data.semaphore == SEM_FAILED)
		return (printf("sem open failed\n"));
	pids = malloc((data.philo_count + 1) * sizeof(int));
	memset(pids, 0, sizeof(int));
	start_child_processes(data, pids);
	while (1)
	{
		i = waitpid(-1, &status, 0);
		if (i == -1)
			break ;
		printf("i am working in main. child exit: %i, status: %i\n", i, status);
	}
	sem_close(data.semaphore);
	free(pids);
	
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
