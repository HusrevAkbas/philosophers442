/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   children.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huakbas <huakbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 13:22:32 by huakbas           #+#    #+#             */
/*   Updated: 2025/05/14 14:59:47 by huakbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	*am_i_dead(void *arg)
{
	t_data	*data;

	data = arg;
	while (1)
	{
		usleep(1000);
		sem_wait(data->sem_mute);
		if (data->return_code)
		{
			sem_post(data->sem_mute);
			return (NULL);
		}
		data->timestamp = ft_get_timestamp(data->last_meal);
		if (data->timestamp > data->time_to_die)
		{
			data->return_code = 1;
			sem_post(data->sem_mute);
			post_all_semaphor_exit(data);
			announce_dead(data);
			return (NULL);
		}
		sem_post(data->sem_mute);
	}
	return (NULL);
}

static void	*wait_for_semaphore(void *arg)
{
	t_data	*data;

	data = arg;
	sem_wait(data->sem_exit);
	sem_wait(data->sem_mute);
	data->return_code = 1;
	sem_post(data->sem_mute);
	while (data->philo_count)
	{
		sem_post(data->sem_exit);
		data->philo_count--;
	}
	return (NULL);
}

static int	change_sem_name(t_data *data, char *semname)
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

static void	open_semaphore_child(t_data *data, char *sem_name)
{
	sem_unlink(sem_name);
	data->sem_mute = sem_open(sem_name, O_CREAT | O_RDWR, 0600, 1);
	if (data->sem_mute == SEM_FAILED)
		exit(3);
	data->sem_exit = sem_open(SEM_EXIT, O_RDWR);
	if (data->sem_exit == SEM_FAILED)
	{
		sem_close(data->sem_mute);
		exit(3);
	}
	data->sem_fork = sem_open(SEM_NAME, O_RDWR);
	if (data->sem_fork == SEM_FAILED)
	{
		sem_close(data->sem_mute);
		sem_close(data->sem_exit);
		exit(3);
	}
}

void	child(t_data data, int *pids)
{
	int		code;
	char	sem_name[5];

	free (pids);
	close_semaphores(data.sem_exit, data.sem_fork, data.sem_mute, 0);
	memset(sem_name, '0', 5);
	change_sem_name(&data, sem_name);
	open_semaphore_child(&data, sem_name);
	usleep(data.name * 100);
	pthread_create(&data.th_wait_semaphore, NULL, wait_for_semaphore, &data);
	pthread_create(&data.th_check_dead, NULL, am_i_dead, &data);
	pthread_detach(data.th_wait_semaphore);
	pthread_detach(data.th_check_dead);
	routine(&data);
	code = data.return_code;
	if (data.return_code == 2)
	{
		usleep(data.time_to_eat * 1100);
		post_all_semaphor_exit(&data);
	}
	usleep(999999);
	close_semaphores(data.sem_exit, data.sem_fork, data.sem_mute, 0);
	sem_unlink(sem_name);
	exit(code);
}
