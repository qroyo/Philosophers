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

int		ft_init_fork(t_entry *data);
int		ft_init_mutex(t_entry *data);
void	ft_free(int check_malloc, t_entry *data);
int		ft_init_malloc(t_entry *data);
void	ft_destroy_init(int check_init, t_entry *data);
void	ft_destroy_fork(t_entry *data, int check_fork);
int		ft_check_init(t_entry *data);

int	ft_init(t_entry *data)
{
	if (ft_check_init(data) != 0)
		return (1);
	data->philo_full = 0;
	return (0);
}

int	ft_check_init(t_entry *data)
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
		ft_free(7, data);
		return (check);
	}
	check = ft_init_fork(data);
	if (check != data->nb_philo)
	{
		ft_destroy_fork(data, check);
		ft_destroy_init(7, data);
		ft_free(7, data);
		return (check);
	}
	return (0);
}

t_philo	ft_init_philo(t_entry *data, t_philo *philo_param)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&data->id_data);
	while (i < data->nb_philo)
	{
		if (data->id_philo[i] == 0)
		{
			(*philo_param).id = i;
			(*philo_param).next_id = i + 1;
			if (i + 1 >= data->nb_philo)
				(*philo_param).next_id = 0;
			data->id_philo[i] = 1;
			break ;
		}
		i++;
	}
	pthread_mutex_unlock(&data->id_data);
	(*philo_param).nb_meal = 0;
	pthread_mutex_lock(&data->start);
	pthread_mutex_unlock(&data->start);
	(*philo_param).think = 0;
	return (*philo_param);
}
