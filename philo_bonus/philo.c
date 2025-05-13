/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huakbas <huakbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:52:30 by husrevakbas       #+#    #+#             */
/*   Updated: 2025/05/13 14:05:30 by huakbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_data(t_data *data, char **args)
{
	data->philo_count = ft_atoi_safe(args[1]);
	data->time_to_die = ft_atoi_safe(args[2]);
	data->time_to_eat = ft_atoi_safe(args[3]);
	data->time_to_sleep = ft_atoi_safe(args[4]);
	if (args[5])
		data->food_max = ft_atoi_safe(args[5]);
	else
		data->food_max = 0;
	data->start_time = ft_now();
	data->return_code = 0;
	data->food_max_reached = 0;
	data->food_counter = 0;
	return (0);
}

void	*am_i_dead(void *arg)
{
	t_data	*data;

	data = arg;
	while (1)
	{
		
		pthread_mutex_lock(&data->mute_data);
		if (data->return_code)
		{
			pthread_mutex_unlock(&data->mute_data);
			return (NULL);
		}
		pthread_mutex_unlock(&data->mute_data);
		usleep(1500);
		pthread_mutex_lock(&data->mute_data);
		data->timestamp = ft_get_timestamp(data->last_meal);
		pthread_mutex_unlock(&data->mute_data);
		if (data->timestamp > data->time_to_die)
		{
			pthread_mutex_lock(&data->mute_data);
			data->return_code = 1;
			pthread_mutex_unlock(&data->mute_data);
			while (data->philo_count)
			{
				sem_post(data->semaphore_exit);
				data->philo_count--;
			}
			if (safe_print(data, "died"))
			{
				data->return_code = 3;
				return (NULL);
			}
			return (NULL);
		}
	}
	return (NULL);
}

void	*wait_for_semaphore(void *arg)
{
	t_data	*data;

	data = arg;
	sem_wait(data->semaphore_exit);
	pthread_mutex_lock(&data->mute_data);
	data->return_code = 1;
	while (data->philo_count)
	{
		sem_post(data->semaphore_exit);
		data->philo_count--;
	}
	pthread_mutex_unlock(&data->mute_data);
	return (NULL);
}

void	child(t_data data, int *pids)
{
	int	code;

	free (pids);
	if (sem_close(data.semaphore_exit) == -1)
	{
		printf("NOT A VALID SEM 1\n");
		exit(3);
	}
	if (sem_close(data.semaphore_fork) == -1)
	{
		printf("NOT A VALID SEM 2\n");
		exit (3);
	}
	data.semaphore_exit = sem_open(SEM_EXIT, O_RDWR);
	if (data.semaphore_exit == SEM_FAILED)
		exit(3);
	data.semaphore_fork = sem_open(SEM_NAME, O_RDWR);
	if (data.semaphore_fork == SEM_FAILED)
	{
		sem_close(data.semaphore_exit);
		exit(3);
	}
	pthread_mutex_init(&data.mute_data, NULL);
	pthread_create(&data.th_wait_semaphore, NULL, wait_for_semaphore, &data);
	pthread_detach(data.th_wait_semaphore);
	pthread_create(&data.th_check_dead, NULL, am_i_dead, &data);
	pthread_detach(data.th_check_dead);
	usleep(data.name * 200);
	routine(&data);
	pthread_mutex_lock(&data.mute_data);
	code = data.return_code;
	if (data.return_code == 2)
	{
		printf("LISTEN TO ME\n");
		usleep(data.time_to_eat * 1100);
		while (data.philo_count)
		{
			sem_post(data.semaphore_exit);
			data.philo_count--;
		}
	}
	pthread_mutex_unlock(&data.mute_data);
	//pthread_join(data.th_check_dead, NULL);
	pthread_mutex_destroy(&data.mute_data);
	sem_close(data.semaphore_exit);
	sem_close(data.semaphore_fork);
	exit(code);
}

int	start_child_processes(t_data data, int *pids)
{
	int	i;
	
	i = 0;
	data.last_meal = ft_now();
	data.start_time = data.last_meal;
	if (data.last_meal == -1)
		return (printf("%s start_child_process\n", GET_TIME_OF_DAY_FAIL));
	while (i < data.philo_count)
	{
		data.name = i + 1;
		pids[i] = fork();
		if (pids[i] == -1)
			return (-1);
		data.pid = pids[i];
		if (pids[i] == 0) //call child process function
			child(data, pids);
		i++;
	}
	return (0);
}

int	terminate_children(pid_t *pids, pid_t current)
{
	int	i;

	i = 0;
	while (pids[i])
	{
		printf("this works\n");
		if (pids[i] != current || pids[i] == current)
			kill(pids[i], SIGQUIT);
		i++;
	}
	return (0);
}

int	main(int argc, char *argv[])
{
	t_data	data;
	pid_t	*pids;
	pid_t	pid;
	int		status;

	if (argc < 5 || argc > 6)
		return (printf("Error: %s", WRONG_ARGUMENT_COUNT));
	ft_check_args(argv);
	if (ft_get_or_set_errors(NULL))
		return (printf(INVALID_ARGUMENT));
	init_data(&data, argv);
	sem_unlink(SEM_NAME); //just to be sure that the name is available
	sem_unlink(SEM_EXIT);
	data.semaphore_fork = sem_open(SEM_NAME, O_CREAT | O_RDWR, 0777, data.philo_count);
	if (data.semaphore_fork == SEM_FAILED)
		return (printf("sem open failed\n"));
	data.semaphore_exit = sem_open(SEM_EXIT, O_CREAT | O_RDWR, 0777, 0);
	if (data.semaphore_exit == SEM_FAILED)
	{
		sem_close(data.semaphore_fork);
		unlink(SEM_NAME);
		return (printf("sem open failed\n"));
	}
	pids = malloc((data.philo_count + 1) * sizeof(int));
	if (!pids)
	{
		sem_close(data.semaphore_fork);
		sem_close(data.semaphore_exit);
		unlink(SEM_NAME);
		unlink(SEM_EXIT);
		return (printf("%s main", MALLOC_FAIL));
	}
	memset(pids, 0, sizeof(int));
	if (start_child_processes(data, pids) == -1)
	{
		status = 0;
		while (status < data.philo_count)
		{
			sem_post(data.semaphore_exit);
			status++;
		}
	}
	while (1)
	{
		// exit 1 when dead - exit 2 when full - exit 3 when error
		pid = waitpid(-1, &status, 0);
		if (pid == -1)
			break ;
		if (WEXITSTATUS(status) == 2)
			continue ;
		printf("i am working in main. child exit: %i, status: %i\n", pid, status);
	}
	sem_close(data.semaphore_fork);
	sem_close(data.semaphore_exit);
	unlink(SEM_NAME);
	unlink(SEM_EXIT);
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
