/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husrevakbas <husrevakbas@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:52:30 by husrevakbas       #+#    #+#             */
/*   Updated: 2025/03/10 13:28:41 by husrevakbas      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char *argv[])
{
	char	*error;
	int		i;

	error = NULL;
	if (argc < 5 || argc > 6)
		ft_get_or_set_errors("Wrong number of arguments\n");
	i = 1;
	while (argv[i])
	{
		int a = ft_atoi_secure(argv[i]);
		if (a == 0 && ft_get_or_set_errors(NULL))
			printf("Argument %d: %s" ,i, ft_get_or_set_errors(NULL));
		i++;
	}
	if (error)
		return (-1);
	return (0);
}
