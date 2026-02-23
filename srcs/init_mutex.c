/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_destroy_mutex.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qroyo <qroyo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 11:37:34 by qroyo             #+#    #+#             */
/*   Updated: 2024/06/24 11:37:35 by qroyo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_init_mutex(t_entry *data)
{
	if (pthread_mutex_init(&data->wrt, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&data->start, NULL) != 0)
		return (2);
	if (pthread_mutex_init(&data->is_alive, NULL) != 0)
		return (3);
	if (pthread_mutex_init(&data->last_eat_m, NULL) != 0)
		return (4);
	if (pthread_mutex_init(&data->id_data, NULL) != 0)
		return (5);
	if (pthread_mutex_init(&data->full_stomach, NULL) != 0)
		return (6);
	return (0);
}

void	ft_destroy_init(int check_init, t_entry *data)
{
	if (check_init >= 2)
		pthread_mutex_destroy(&data->wrt);
	if (check_init >= 3)
		pthread_mutex_destroy(&data->start);
	if (check_init >= 4)
		pthread_mutex_destroy(&data->is_alive);
	if (check_init >= 5)
		pthread_mutex_destroy(&data->last_eat_m);
	if (check_init >= 6)
		pthread_mutex_destroy(&data->id_data);
	if (check_init >= 7)
		pthread_mutex_destroy(&data->full_stomach);
}
