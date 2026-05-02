/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_part_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 15:35:20 by aanouer           #+#    #+#             */
/*   Updated: 2026/05/02 14:27:13 by aanouer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	init_simulation_with_default_values(t_simulation **sim)
{
	(*sim)->number_of_coders = -1;
	(*sim)->time_to_compile = -1;
	(*sim)->time_to_debug = -1;
	(*sim)->time_to_refactor = -1;
	(*sim)->number_of_compiles_required = -1;
	(*sim)->dongle_cooldown = -1;
	(*sim)->scheduler = "";
}

static int	init_queue_with_default_values(t_queue **queue)
{
	(*queue)->number_of_tickets = 0;
	(*queue)->max_nums_of_tickets = 2;
	(*queue)->requests = malloc(sizeof(t_request) * 2);
	if (!(*queue)->requests)
		return (0);
	return (1);
}

int	init_dongles_and_coders(t_simulation **sim)
{
	(*sim)->coders = malloc(sizeof(t_coder) * (*sim)->number_of_coders);
	if (!(*sim)->coders)
		return (0);
	(*sim)->dongles = malloc(sizeof(t_dongle) * (*sim)->number_of_coders);
	if (!(*sim)->dongles)
		return (free((*sim)->coders), (*sim)->coders = NULL, 0);
	return (1);
}

int	init_queue_of_dongles(t_dongle **dongles, long number_of_coders)
{
	long	i;

	i = 0;
	while (i < number_of_coders)
	{
		(*dongles)[i].queue = malloc(sizeof(t_queue));
		if (!(*dongles)[i].queue)
			return (free_queues(dongles, i), 0);
		if (!init_queue_with_default_values(&(*dongles)[i].queue))
			return (free((*dongles)[i].queue), free_queues(dongles, i), 0);
		i++;
	}
	return (1);
}

int	init_coders(t_simulation *sim)
{
	long	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		sim->coders[i].id = i + 1;
		sim->coders[i].compile_count = 0;
		sim->coders[i].last_compile_start = get_current_time();
		if (pthread_mutex_init(&sim->coders[i].lock_l_c_s, NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&sim->coders[i].lock_l_c_s);
			return (0);
		}
		sim->coders[i].left_dongle = &sim->dongles[i];
		sim->coders[i].right_dongle = (&sim->dongles[(i + 1)
				% sim->number_of_coders]);
		sim->coders[i].sim = sim;
		i++;
	}
	return (1);
}
