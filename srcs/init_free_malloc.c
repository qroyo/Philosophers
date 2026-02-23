/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_free_malloc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qroyo <qroyo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 11:40:45 by qroyo             #+#    #+#             */
/*   Updated: 2024/06/24 11:40:46 by qroyo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_init_malloc(t_entry *data)
{
	data->philo_tab = malloc(data->nb_philo * sizeof(pthread_t));
	if (!data->philo_tab)
		return (1);
	data->fork = malloc(data->nb_philo * sizeof(pthread_mutex_t));
	if (!data->fork)
		return (2);
	data->last_eat = malloc(data->nb_philo * sizeof(struct timeval));
	if (!data->last_eat)
		return (3);
	data->fork_philo = malloc(data->nb_philo * sizeof(int));
	if (!data->fork_philo)
		return (4);
	data->id_philo = malloc(data->nb_philo * sizeof(int));
	if (!data->id_philo)
		return (5);
	return (0);
}

void	ft_free(int check_malloc, t_entry *data)
{
	if (check_malloc >= 2)
		free(data->philo_tab);
	if (check_malloc >= 3)
		free(data->fork);
	if (check_malloc >= 4)
		free(data->last_eat);
	if (check_malloc >= 5)
		free(data->fork_philo);
	if (check_malloc >= 6)
		free(data->id_philo);
}
