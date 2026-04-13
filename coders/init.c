/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 09:14:25 by aanouer           #+#    #+#             */
/*   Updated: 2026/04/13 13:18:46 by aanouer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	free_waiting_queue(t_dongle **dongles, int i)
{
	int	j;

	j = 0;
	while (j < i)
	{
		free((*dongles)[j].waiting_queue);
		j++;
	}
}

int	initialize_dongles(t_simulation *sim, t_dongle *dongles)
{
	int	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		pthread_mutex_init(&dongles[i].mutex, NULL);
		pthread_cond_init(&dongles[i].condition, NULL);
		dongles[i].cooldown_until = 0;
		dongles[i].is_available = 1;
		dongles[i].waiting_queue = (malloc(sizeof(t_coder *)
					* sim->number_of_coders));
		if (!dongles[i].waiting_queue)
		{
			free_waiting_queue(&dongles, i);
			return (0);
		}
		memset(dongles[i].waiting_queue, 0,
			sizeof(t_coder *) * sim->number_of_coders);
		dongles[i].waiting_capacity = sim->number_of_coders;
		dongles[i].waiting_count = 0;
		i++;
	}
	return (1);
}

void	initialize_mutexes(t_simulation *sim)
{
	struct timeval	tv;

	pthread_mutex_init(&sim->print_mutex, NULL);
	pthread_mutex_init(&sim->stop_mutex, NULL);
	sim->stop_simulation = 0;
	gettimeofday(&tv, NULL);
	sim->start_time = (long long)tv.tv_sec * 1000000LL + tv.tv_usec;
}

void	initialize_coders(t_simulation *sim, t_coder *coders,
	t_dongle *dongles)
{
	int	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		coders[i].id = i + 1;
		coders[i].compile_count = 0;
		coders[i].last_compile_time = sim->start_time;
		coders[i].deadline = sim->start_time + (sim->time_to_burnout * 1000);
		coders[i].sim = sim;
		coders[i].left_dongle = &dongles[i];
		coders[i].right_dongle = &dongles[(i + 1) % sim->number_of_coders];
		i++;
	}
}
