/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 09:14:25 by aanouer           #+#    #+#             */
/*   Updated: 2026/03/31 09:14:52 by aanouer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	initialize_dongles(t_simulation *sim, t_dongle *dongles)
{
	int	i;

	dongles = malloc(sizeof(t_dongle) * sim->number_of_coders);
	while (i < sim->number_of_coders)
	{
		pthread_mutex_init(&dongles[i], NULL);
		pthread_cond_init(&dongles[i].condition, NULL);
		dongles[i].cooldown_until = 0;
		dongles[i].is_available = 1;
		i += 1;
	}
}

static void	initialize_mutexes(t_simulation *sim)
{
	struct timeval	tv;

	pthread_mutex_init(&sim->print_mutex, NULL);
	pthread_mutex_init(&sim->stop_mutex, NULL);
	sim->stop_simulation = 0;
	gettimeofday(&tv, NULL);
	sim->start_time = (long long)tv.tv_sec * 1000000LL + tv.tv_usec;
}
