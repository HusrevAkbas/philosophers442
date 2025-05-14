/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huakbas <huakbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:52:30 by husrevakbas       #+#    #+#             */
/*   Updated: 2025/05/14 13:34:21 by huakbas          ###   ########.fr       */
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

int	open_semaphore_main(t_data *data)
{
	char	*name;

	name = "mainmuter";
	sem_unlink(SEM_NAME);
	sem_unlink(SEM_EXIT);
	data->sem_fork = sem_open(SEM_NAME, O_CREAT | O_RDWR,
			0600, data->philo_count);
	if (data->sem_fork == SEM_FAILED)
		return (printf("%s open_semaphore_main (1)\n", SEM_OPEN_FAIL));
	data->sem_exit = sem_open(SEM_EXIT, O_CREAT | O_RDWR, 0600, 0);
	if (data->sem_exit == SEM_FAILED)
	{
		sem_close(data->sem_fork);
		sem_unlink(SEM_NAME);
		return (printf("%s open_semaphore_main (2)\n", SEM_OPEN_FAIL));
	}
	data->sem_mute = sem_open(name, O_CREAT | O_RDWR, 0600, 1);
	if (data->sem_mute == SEM_FAILED)
	{
		close_semaphores(data->sem_exit, data->sem_fork, NULL, 1);
		return (printf("%s open_semaphore_main (3)\n", SEM_OPEN_FAIL));
	}
	return (0);
}

void	wait_children(t_data *data)
{
	int	status;
	int	pid;

	while (1)
	{
		pid = waitpid(-1, &status, 0);
		if (pid == -1)
			break ;
		if (WEXITSTATUS(status) == 2)
			continue ;
		if (WEXITSTATUS(status) == 3)
		{
			post_all_semaphor_exit(data);
			printf("AN ERROR OCCURED IN CHILD PROCESS ID: %i - Aborting...\n",
				pid);
		}
	}
}

int	main(int argc, char *argv[])
{
	t_data	data;
	pid_t	*pids;

	if (argc < 5 || argc > 6)
		return (printf("Error: %s", WRONG_ARGUMENT_COUNT));
	ft_check_args(argv);
	if (ft_get_or_set_errors(NULL))
		return (printf(INVALID_ARGUMENT));
	init_data(&data, argv);
	if (open_semaphore_main(&data))
		return (3);
	pids = malloc((data.philo_count + 1) * sizeof(int));
	if (!pids)
	{
		close_semaphores(data.sem_exit, data.sem_fork, NULL, 1);
		return (printf("%s main", MALLOC_FAIL));
	}
	memset(pids, 0, sizeof(int));
	if (start_child_processes(data, pids) == -1)
		post_all_semaphor_exit(&data);
	wait_children(&data);
	close_semaphores(data.sem_exit, data.sem_fork,
		data.sem_mute, 1);
	free(pids);
	return (0);
}
