/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huakbas <huakbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:51:01 by husrevakbas       #+#    #+#             */
/*   Updated: 2025/05/07 18:06:17 by huakbas          ###   ########.fr       */
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

//ERRORS
# define WRONG_ARGUMENT_COUNT	"Error: Wrong number of arguments\n"
# define NEGATIVE_NUMBER		"Arguments must be a positive integer\n"
# define INVALID_ARGUMENT		"Error: Invalid argument(s)\n"
# define MALLOC_FAIL			"Error: malloc failed\n"
# define NUMBER_GREATER_THAN_INT_MAX	"Argument is greater than INT_MAX\n"
# define NUMBER_LOWER_THAN_INT_MIN		"Argument is lower than INT_MIN\n"
# define NOT_A_NUMBER			"Argument is not a number\n"
# define ERROR_CREATING_THREAD	"Error on creating threads\n"

typedef struct s_philo
{
	int				name;
	int				hungry;
	int				food_counter;
	int				timestamp;
	struct s_data	*data;
	long long		last_meal;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	long long		start_time;
	pthread_t		thread;
	pthread_mutex_t	mute_fork;
	pthread_mutex_t	*mute_fork2;
}	t_philo;

typedef struct s_data
{
	int				*who_is_dead;
	int				philo_count;
	int				food_max;
	int				*food_max_reached;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	long long		start_time;
	pthread_mutex_t	mute_data;
	pthread_mutex_t	mute_print;
}	t_data;

//Use NULL as argument to get errors. 
//Use a text as argument to add an error.
//Returns NULL when setting errors or if there is no error
char		*ft_get_or_set_errors(char *error);
void		ft_check_args(char **argv);
int			ft_atoi_safe(char *num);
int			ft_get_timestamp(long long old);
int			ft_isdigit(int c);
// always use with a message
//otherwise it may return a pointer
void		*ft_free_many(void *one, void *two, void *three, char *message);
int			safe_print(t_philo *philo, char *message);
void		*routine(void	*arg);
int			is_somone_dead_or_food_max_reached(t_data *data);
t_philo		**init_philos(t_data *data);
t_philo		**go_to_bath(t_philo **philos, t_data *data);
long long	ft_now(void);
int			unlock_mutex_return_one(pthread_mutex_t *mutex1,
				pthread_mutex_t *mutex2);

#endif //PHILO_H