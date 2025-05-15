/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huakbas <huakbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:51:01 by husrevakbas       #+#    #+#             */
/*   Updated: 2025/05/15 15:56:44 by huakbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# define _GNU_SOURCE

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <signal.h>
# include <string.h>
# include <fcntl.h>
# include <semaphore.h>
# include <pthread.h>
# include <sys/time.h>
# include <sys/wait.h>

//ERRORS
# define WRONG_ARGUMENT_COUNT	"Error: Wrong number of arguments\n"
# define NEGATIVE_NUMBER		"Arguments must be a positive integer\n"
# define INVALID_ARGUMENT		"Error: Invalid argument(s)\n"
# define MALLOC_FAIL			"Error: malloc failed in"
# define NUMBER_GREATER_THAN_INT_MAX	"Argument is greater than INT_MAX\n"
# define NUMBER_LOWER_THAN_INT_MIN		"Argument is lower than INT_MIN\n"
# define NOT_A_NUMBER			"Argument is not a number\n"
# define ERROR_CREATING_THREAD	"Error on creating threads\n"
# define GET_TIME_OF_DAY_FAIL	"get_time_of_day failed in"
# define SEM_OPEN_FAIL			"sem_open failed in"
# define BIG_NUMBER_FOR_PHLOS	"You cant use numbers more than\
 1000 as count of philosophers\n"

# define SEM_NAME				"MYBRANDNEWSEMAPHOREhuakbas"
# define SEM_EXIT				"MYSEMAPHORETOEXIThuakbas"

typedef struct s_data
{
	int				name;
	int				food_counter;
	int				return_code;
	int				philo_count;
	int				food_max;
	long long		last_meal;
	int				timestamp;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	long long		start_time;
	pid_t			pid;
	sem_t			*sem_fork;
	sem_t			*sem_exit;
	sem_t			*sem_mute;
	pthread_t		th_wait_semaphore;
	pthread_t		th_check_dead;
}	t_data;

//Use NULL as argument to get errors. 
//Use a text as argument to add an error.
//Returns NULL when setting errors or if there is no error
char		*ft_get_or_set_errors(char *error);
void		ft_check_args(char **argv);
int			ft_atoi_safe(char *num);
int			ft_get_timestamp(long long old);
int			safe_print(t_data *data, char *message);
int			routine(t_data *data);
long long	ft_now(void);
int			unlock_mutex_return_one(pthread_mutex_t *mutex1,
				pthread_mutex_t *mutex2);
void		close_semaphores(sem_t *sem1, sem_t *sem2, sem_t *sem3, int unlink);
int			announce_dead(t_data *data);
void		child(t_data data, int *pids);
void		post_all_semaphor_exit(t_data *data);
void		sleep_disorder(t_data *data, int time);

#endif //PHILO_H