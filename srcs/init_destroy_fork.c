/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_destroy_fork.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qroyo <qroyo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 11:35:59 by qroyo             #+#    #+#             */
/*   Updated: 2024/06/24 11:36:00 by qroyo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_init_fork(t_entry *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philo)
	{
		if (pthread_mutex_init(&data->fork[i], NULL) != 0)
			return (i);
		data->fork_philo[i] = 0;
		data->id_philo[i] = 0;
		i++;
	}
	return (i);
}

void	ft_destroy_fork(t_entry *data, int check_fork)
{
	int	i;

	i = 0;
	while (i < check_fork)
	{
		pthread_mutex_destroy(&data->fork[i]);
		i++;
	}
}
