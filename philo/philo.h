/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husrevakbas <husrevakbas@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:51:01 by husrevakbas       #+#    #+#             */
/*   Updated: 2025/03/10 13:21:14 by husrevakbas      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <stdio.h>
# include <stdlib.h>
# include <limits.h>
# include <string.h>

// ERROR CODES
typedef struct s_error
{
	int				code;
	char			*message;
	struct s_error	*next;
}	t_error;

//Use NULL as argument to get errors. 
//Use a text as argument to add an error.
//Returns NULL when setting errors and if there is no error
char	*ft_get_or_set_errors(char *error);
int		ft_print_errors(char **err);
int		ft_atoi_secure(char *num);
int		ft_isdigit(int c);

#endif //PHILO_H