/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philos.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huakbas <huakbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 16:06:06 by huakbas           #+#    #+#             */
/*   Updated: 2025/05/07 18:52:29 by huakbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// t_philo	**go_to_bath(t_philo **philos, t_data *data)
// {
// 	int	i;

// 	if (data)
// 	{
// 		free(data->food_max_reached);
// 		free(data->who_is_dead);
// 		free(data);
// 		data = NULL;
// 	}
// 	if (!philos)
// 		return (NULL);
// 	i = 0;
// 	while (philos[i])
// 	{
// 		free (philos[i]);
// 		i++;
// 	}
// 	free (philos);
// 	return (NULL);
// }

void	init_philos(t_philo *philos, t_data *data)
{
	memset(philos, 0, sizeof(t_philo));
	philos->data = data;
	philos->time_to_die = data->time_to_die;
	philos->time_to_eat = data->time_to_eat;
	philos->time_to_sleep = data->time_to_sleep;
	philos->start_time = data->start_time;
}
