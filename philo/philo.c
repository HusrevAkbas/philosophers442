/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husrevakbas <husrevakbas@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:52:30 by husrevakbas       #+#    #+#             */
/*   Updated: 2025/03/11 10:57:41 by husrevakbas      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char *argv[])
{
	int		i;

	if (argc < 5 || argc > 6)
		return (printf("Error: %s", WRONG_ARGUMENT_COUNT));
	i = 1;
	while (argv[i])
	{
		int a = ft_atoi_secure(argv[i]);
		if (a < 0)
		{
			ft_get_or_set_errors(NEGATIVE_NUMBER);
			printf("Argument %d: %s", i, NEGATIVE_NUMBER);
		}
		if (a == 0 && ft_get_or_set_errors(NULL))
			printf("Argument %d: %s" ,i, ft_get_or_set_errors(NULL));
		i++;
	}
	if (ft_get_or_set_errors(NULL))
		return (printf("Error: Invalid argument(s)\n"));
	return (0);
}
