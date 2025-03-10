/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husrevakbas <husrevakbas@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 11:39:12 by husrevakbas       #+#    #+#             */
/*   Updated: 2025/03/10 13:25:06 by husrevakbas      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

char	*ft_get_or_set_errors(char *error)
{
	static char *err;

	if (!error)
		return (err);
	err = error;
	return	(NULL);
}
