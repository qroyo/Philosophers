/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qroyo <qroyo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:41:38 by qroyo             #+#    #+#             */
/*   Updated: 2024/02/20 11:41:39 by qroyo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

static long int	get_num(const char *str, size_t i, int minus)
{
	long int	nb;

	nb = 0;
	while (ft_isdigit(str[i]))
	{
		if (((nb * 10) + str[i] - '0') / 10 != nb)
		{
			if (minus == -1)
				return (LONG_MIN);
			else
				return (LONG_MAX);
		}
		nb = (nb * 10) + (str[i] - '0');
		i++;
	}
	return (minus * nb);
}

long int	ft_atoi(const char *str)
{
	size_t	i;
	int		minus;

	i = 0;
	minus = 1;
	while (str[i] == '\t' || str[i] == '\n' || str[i] == '\v'
		|| str[i] == '\f' || str[i] == '\r' || str[i] == ' ')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			minus = -1;
		i++;
	}
	return (get_num(str, i, minus));
}

int	ft_check_digit(int argc, char **argv)
{
	int	i;
	int	j;

	i = 1;
	while (i < argc)
	{
		if (argv[i][0] == '\0')
		{
			printf("Empty parameter\n");
			return (1);
		}
		j = 0;
		while (argv[i][j])
		{
			if (ft_isdigit(argv[i][j]) != 1)
			{
				printf("Non-digit parameter or negative number\n");
				return (2);
			}
			j++;
		}
		i++;
	}
	return (0);
}

t_entry	*ft_parse(int argc, char **argv)
{
	t_entry	*data;

	if (ft_check_digit(argc, argv) != 0)
		return (NULL);
	if (ft_atoi(argv[1]) == 0)
	{
		printf("Number of philosophers must be at least 1\n");
		return (NULL);
	}
	if (ft_atoi(argv[1]) > MAX_PHILOSOPHERS)
	{
		printf("Too many philosophers (max %d)\n", MAX_PHILOSOPHERS);
		return (NULL);
	}
	if (ft_atoi(argv[2]) <= 0 || ft_atoi(argv[3]) <= 0
		|| ft_atoi(argv[4]) <= 0)
	{
		printf("Time values must be positive\n");
		return (NULL);
	}
	if (argc == 6 && ft_atoi(argv[5]) <= 0)
	{
		printf("Number of meals must be positive\n");
		return (NULL);
	}
	data = malloc(sizeof(t_entry));
	if (!data)
		return (NULL);
	data->nb_philo = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->nb_meal = ft_atoi(argv[5]);
	else
		data->nb_meal = -1;
	data->alive = 1;
	return (data);
}
