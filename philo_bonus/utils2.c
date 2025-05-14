/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huakbas <huakbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 11:39:12 by husrevakbas       #+#    #+#             */
/*   Updated: 2025/05/14 13:34:21 by huakbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

char	*ft_get_or_set_errors(char *error)
{
	static char	*err;

	if (!error)
		return (err);
	err = error;
	return (NULL);
}

int	announce_dead(t_data *data)
{
	if (data->philo_count == 1)
		sem_post(data->sem_fork);
	if (safe_print(data, "died"))
	{
		sem_wait(data->sem_mute);
		data->return_code = 3;
		sem_post(data->sem_mute);
		return (3);
	}
	return (0);
}

void	post_all_semaphor_exit(t_data *data)
{
	int	i;

	i = 0;
	sem_wait(data->sem_mute);
	while (i < data->philo_count)
	{
		sem_post(data->sem_exit);
		i++;
	}
	sem_post(data->sem_mute);
}
