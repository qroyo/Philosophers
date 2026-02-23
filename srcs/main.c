/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qroyo <qroyo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:41:31 by qroyo             #+#    #+#             */
/*   Updated: 2024/02/20 11:41:34 by qroyo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_clean_philo(t_entry *data);
int		ft_create_philo(t_entry *data);
void	ft_philo_join(t_entry *data);
int		ft_check_create_watch(t_entry *data);

int	main(int argc, char **argv)
{
	t_entry	*data;

	if (argc == 5 || argc == 6)
	{
		data = ft_parse(argc, argv);
		if (!data)
			return (2);
		if (ft_init(data) != 0)
			return (1);
		if (ft_create_philo(data) == 1 || ft_check_create_watch(data) == 1)
		{
			ft_clean_philo(data);
			return (3);
		}
		ft_philo_join(data);
		ft_clean_philo(data);
	}
	else
	{
		printf("4/5 args needed\n");
		return (1);
	}
	return (0);
}

void	ft_philo_join(t_entry *data)
{
	int				i;
	struct timeval	time;

	i = 0;
	gettimeofday(&time, NULL);
	data->time_start = time;
	while (i < data->nb_philo)
	{
		data->last_eat[i] = time;
		i++;
	}
	pthread_mutex_unlock(&data->start);
	i = 0;
	while (i < data->nb_philo)
	{
		pthread_join(data->philo_tab[i], NULL);
		i++;
	}
	pthread_join(data->check_philo, NULL);
}

int	ft_create_philo(t_entry *data)
{
	int	i;
	int	j;

	i = -1;
	j = 0;
	pthread_mutex_lock(&data->start);
	while (++i < data->nb_philo)
	{
		if (pthread_create(data->philo_tab + i, NULL, \
		eat_sleep_think, data) != 0)
		{
			while (j < i)
			{
				pthread_detach(data->philo_tab[j]);
				j++;
			}
			data->alive = 0;
			return (1);
		}
	}
	return (0);
}

int	ft_check_create_watch(t_entry *data)
{
	int	j;

	j = 0;
	if (pthread_create(&data->check_philo, NULL, check_philo, data) != 0)
	{
		while (j < data->nb_philo)
		{
			pthread_detach(data->philo_tab[j]);
			j++;
		}
		return (1);
	}
	return (0);
}

void	ft_clean_philo(t_entry *data)
{
	int	i;

	i = -1;
	pthread_mutex_destroy(&data->start);
	pthread_mutex_destroy(&data->wrt);
	pthread_mutex_destroy(&data->is_alive);
	pthread_mutex_destroy(&data->last_eat_m);
	pthread_mutex_destroy(&data->id_data);
	pthread_mutex_destroy(&data->full_stomach);
	while (++i < data->nb_philo)
		pthread_mutex_destroy(&data->fork[i]);
	free(data->fork);
	free(data->next_id);
	free(data->philo_tab);
	free(data->last_eat);
	free(data->fork_philo);
	free(data->id_philo);
	free(data);
}
