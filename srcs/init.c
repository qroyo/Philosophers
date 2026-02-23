/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qroyo <qroyo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 13:45:20 by qroyo             #+#    #+#             */
/*   Updated: 2024/03/07 13:45:21 by qroyo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_check_init(t_entry *data);

int	ft_init(t_entry *data)
{
	if (ft_check_init(data) != 0)
		return (1);
	data->philo_full = 0;
	data->stop = 0;
	return (0);
}

static int	ft_check_init(t_entry *data)
{
	int	check;

	check = ft_init_malloc(data);
	if (check != 0)
	{
		ft_free(check, data);
		return (check);
	}
	check = ft_init_mutex(data);
	if (check != 0)
	{
		ft_destroy_init(check, data);
		ft_free(6, data);
		return (check);
	}
	check = ft_init_fork(data);
	if (check != data->nb_philo)
	{
		ft_destroy_fork(data, check);
		ft_destroy_init(7, data);
		ft_free(6, data);
		return (check);
	}
	return (0);
}

/*
** Assigns a unique ID to each philosopher thread.
** Uses id_data mutex to ensure thread-safe ID assignment.
** Waits on start mutex as a synchronization barrier so all
** threads begin simultaneously once the main thread releases it.
*/
void	ft_init_philo(t_entry *data, t_philo *philo_param)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&data->id_data);
	while (i < data->nb_philo)
	{
		if (data->id_philo[i] == 0)
		{
			philo_param->id = i;
			philo_param->next_id = i + 1;
			if (i + 1 >= data->nb_philo)
				philo_param->next_id = 0;
			data->id_philo[i] = 1;
			break ;
		}
		i++;
	}
	pthread_mutex_unlock(&data->id_data);
	philo_param->nb_meal = 0;
	philo_param->think = 0;
	pthread_mutex_lock(&data->start);
	pthread_mutex_unlock(&data->start);
}
