/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husrevakbas <husrevakbas@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:51:01 by husrevakbas       #+#    #+#             */
/*   Updated: 2025/03/11 11:59:54 by husrevakbas      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <stdio.h>
# include <stdlib.h>
# include <limits.h>
# include <string.h>
# include <pthread.h>

//ERRORS
# define WRONG_ARGUMENT_COUNT "Wrong number of arguments\n"
# define NEGATIVE_NUMBER "Arguments must be a positive integer\n"

typedef struct s_philo
{
	int	name;
	pthread_mutex_t	fork;
	pthread_mutex_t	*fork;
};

//Use NULL as argument to get errors. 
//Use a text as argument to add an error.
//Returns NULL when setting errors and if there is no error
char	*ft_get_or_set_errors(char *error);
int		ft_atoi_secure(char *num);
int		ft_isdigit(int c);

#endif //PHILO_H