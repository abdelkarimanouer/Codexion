/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_part_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 06:23:59 by aanouer           #+#    #+#             */
/*   Updated: 2026/05/02 09:37:00 by aanouer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	handle_dongle_mutex_fail(t_simulation *sim, long i)
{
	pthread_mutex_destroy(&sim->stop_mutex);
	pthread_mutex_destroy(&sim->print_mutex);
	pthread_mutex_destroy(&sim->ticket_count_mutex);
	while (--i >= 0)
		pthread_mutex_destroy(&sim->dongles[i].lock_dongle);
	return (0);
}

static int	handle_dongle_cond_fail(t_simulation *sim, long i)
{
	pthread_mutex_destroy(&sim->dongles[i].lock_dongle);
	while (--i >= 0)
	{
		pthread_mutex_destroy(&sim->dongles[i].lock_dongle);
		pthread_cond_destroy(&sim->dongles[i].cond_dongle);
	}
	return (0);
}

static int	init_dongle_mutexes(t_simulation *sim)
{
	long	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		if (pthread_mutex_init(&sim->dongles[i].lock_dongle, NULL) != 0)
			return (handle_dongle_mutex_fail(sim, i));
		if (pthread_cond_init(&sim->dongles[i].cond_dongle, NULL) != 0)
			return (handle_dongle_cond_fail(sim, i));
		sim->dongles[i].is_available = 1;
		sim->dongles[i].cooldown_end = 0;
		i++;
	}
	return (1);
}

static int	init_sync_mutexes(t_simulation *sim)
{
	if (pthread_mutex_init(&sim->sync_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&sim->stop_mutex);
		pthread_mutex_destroy(&sim->print_mutex);
		pthread_mutex_destroy(&sim->ticket_count_mutex);
		return (0);
	}
	if (pthread_cond_init(&sim->sync_cond, NULL) != 0)
	{
		pthread_mutex_destroy(&sim->stop_mutex);
		pthread_mutex_destroy(&sim->print_mutex);
		pthread_mutex_destroy(&sim->ticket_count_mutex);
		pthread_mutex_destroy(&sim->sync_mutex);
		return (0);
	}
	return (1);
}

int	init_mutexes_and_dongles(t_simulation *sim)
{
	if (pthread_mutex_init(&sim->stop_mutex, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&sim->print_mutex, NULL) != 0)
		return (pthread_mutex_destroy(&sim->stop_mutex), 0);
	if (pthread_mutex_init(&sim->ticket_count_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&sim->stop_mutex);
		pthread_mutex_destroy(&sim->print_mutex);
		return (0);
	}
	if (!init_sync_mutexes(sim))
		return (0);
	sim->stop = 0;
	sim->ticket_count = 0;
	sim->threads_ready = 0;
	return (init_dongle_mutexes(sim));
}
