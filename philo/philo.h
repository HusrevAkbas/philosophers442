/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husrevakbas <husrevakbas@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:51:01 by husrevakbas       #+#    #+#             */
/*   Updated: 2025/03/14 14:47:03 by husrevakbas      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <string.h>
# include <pthread.h>
# include <sys/time.h>

# define t_timeval struct timeval

//ERRORS
# define WRONG_ARGUMENT_COUNT	"Error: Wrong number of arguments\n"
# define NEGATIVE_NUMBER		"Arguments must be a positive integer\n"
# define INVALID_ARGUMENT		"Error: Invalid argument(s)\n"
# define MALLOC_FAIL			"Error: malloc failed\n"
# define NUMBER_GREATER_THAN_INT_MAX	"Argument is greater than INT_MAX\n"
# define NUMBER_LOWER_THAN_INT_MIN		"Argument is lower than INT_MIN\n"
# define NOT_A_NUMBER			"Argument is not a number\n"

typedef struct s_philo
{
	int				name;
	int				fork;
	int				*fork2;
	int				hungry;
	int				*who_is_dead;
	int				philo_count;
	int				food_counter;
	int				food_max;
	int				*food_max_reached;
	//int				sleepy;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	// time of start or last meal
	t_timeval		last_meal;
	t_timeval		start_time;
	pthread_t		*thread;
	pthread_mutex_t	*mute_fork;
	pthread_mutex_t	*mute_fork2;
	//struct s_philo	*next;
}	t_philo;

typedef struct s_queue
{
	int				philo_index;
	struct s_queue	*next;
}	t_queue;


//Use NULL as argument to get errors. 
//Use a text as argument to add an error.
//Returns NULL when setting errors or if there is no error
void	ft_check_args(char **argv);
int		ft_atoi_safe(char *num);
char	*ft_get_or_set_errors(char *error);
int		ft_get_timestamp(t_timeval tv);
int		ft_isdigit(int c);
int		ft_update_last_meal(t_timeval *tv);

#endif //PHILO_H