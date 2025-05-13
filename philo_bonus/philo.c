/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huakbas <huakbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:52:30 by husrevakbas       #+#    #+#             */
/*   Updated: 2025/05/13 17:39:18 by huakbas          ###   ########.fr       */
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

void	post_all_semaphor_exit(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		sem_post(data->semaphore_exit);
		i++;
	}
}

void	*am_i_dead(void *arg)
{
	t_data	*data;

	data = arg;
	while (1)
	{
		usleep(1000);
		sem_wait(data->semaphore_mute);
		if (data->return_code)
		{
			sem_post(data->semaphore_mute);
			return (NULL);
		}
		data->timestamp = ft_get_timestamp(data->last_meal);
		if (data->timestamp > data->time_to_die)
		{
			data->return_code = 1;
			post_all_semaphor_exit(data);
			sem_post(data->semaphore_mute);
			if (data->philo_count == 1)
				sem_post(data->semaphore_fork);
			if (safe_print(data, "died"))
			{
				sem_wait(data->semaphore_mute);
				data->return_code = 3;
				sem_post(data->semaphore_mute);
				return (NULL);
			}
			return (NULL);
		}
		sem_post(data->semaphore_mute);
	}
	return (NULL);
}

void	*wait_for_semaphore(void *arg)
{
	t_data	*data;

	data = arg;
	sem_wait(data->semaphore_exit);
	sem_wait(data->semaphore_mute);
	data->return_code = 1;
	sem_post(data->semaphore_mute);
	while (data->philo_count)
	{
		sem_post(data->semaphore_exit);
		data->philo_count--;
	}
	return (NULL);
}

int	change_sem_name(t_data *data, char *semname)
{
	int	i;
	int	name;

	name = data->name;
	i = 0;
	semname[4] = 0;
	while (name % 10 > 0 && i < 5)
	{
		semname[i] = (name % 10) + 48;
		i++;
		name = name / 10;
	}
	semname[4] = 0;
	return (0);
}

void	child(t_data data, int *pids)
{
	int		code;
	char	sem_name[5];

	free (pids);
	sem_close(data.semaphore_exit);
	sem_close(data.semaphore_fork);
	memset(sem_name, '0', 5);
	change_sem_name(&data, sem_name);
	sem_unlink(sem_name);
	data.semaphore_mute = sem_open(sem_name, O_CREAT | O_RDWR, 0600, 1);
	if (data.semaphore_mute == SEM_FAILED)
		exit(3);
	data.semaphore_exit = sem_open(SEM_EXIT, O_RDWR);
	if (data.semaphore_exit == SEM_FAILED)
	{
		sem_close(data.semaphore_mute);
		exit(3);
	}
	data.semaphore_fork = sem_open(SEM_NAME, O_RDWR);
	if (data.semaphore_fork == SEM_FAILED)
	{
		sem_close(data.semaphore_mute);
		sem_close(data.semaphore_exit);
		exit(3);
	}
	pthread_create(&data.th_wait_semaphore, NULL, wait_for_semaphore, &data);
	pthread_detach(data.th_wait_semaphore);
	pthread_create(&data.th_check_dead, NULL, am_i_dead, &data);
	pthread_detach(data.th_check_dead);
	usleep(data.name * 100);
	routine(&data);
	sem_wait(data.semaphore_mute);
	code = data.return_code;
	if (data.return_code == 2)
	{
		usleep(data.time_to_eat * 1100);
		post_all_semaphor_exit(&data);
	}
	sem_post(data.semaphore_mute);
	sem_close(data.semaphore_exit);
	sem_close(data.semaphore_fork);
	sem_close(data.semaphore_mute);
	sem_unlink(sem_name);
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
		if (pids[i] == 0)
			child(data, pids);
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
	sem_unlink(SEM_NAME);
	sem_unlink(SEM_EXIT);
	data.semaphore_fork = sem_open(SEM_NAME, O_CREAT | O_RDWR, 0600, data.philo_count);
	if (data.semaphore_fork == SEM_FAILED)
		return (printf("sem open failed\n"));
	data.semaphore_exit = sem_open(SEM_EXIT, O_CREAT | O_RDWR, 0600, 0);
	if (data.semaphore_exit == SEM_FAILED)
	{
		sem_close(data.semaphore_fork);
		sem_unlink(SEM_NAME);
		return (printf("sem open failed\n"));
	}
	pids = malloc((data.philo_count + 1) * sizeof(int));
	if (!pids)
	{
		sem_close(data.semaphore_fork);
		sem_close(data.semaphore_exit);
		sem_unlink(SEM_NAME);
		sem_unlink(SEM_EXIT);
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
		pid = waitpid(-1, &status, 0);
		if (pid == -1)
			break ;
		if (WEXITSTATUS(status) == 2)
			continue ;
	}
	sem_close(data.semaphore_fork);
	sem_close(data.semaphore_exit);
	sem_unlink(SEM_NAME);
	sem_unlink(SEM_EXIT);
	free(pids);
	return (0);
}
