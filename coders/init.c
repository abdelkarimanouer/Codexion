/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 15:35:20 by aanouer           #+#    #+#             */
/*   Updated: 2026/04/20 05:49:21 by aanouer          ###   ########.fr       */
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

static void	init_queue_with_default_values(t_queue **queue, long n_of_coders)
{
	(*queue)->number_of_tickets = 0;
	(*queue)->max_nums_of_tickets = n_of_coders;
}

int	init_dongles_and_coders(t_simulation **sim)
{
	(*sim)->coders = malloc(sizeof(t_coder) * (*sim)->number_of_coders);
	if (!(*sim)->coders)
		return (0);
	(*sim)->dongles = malloc(sizeof(t_dongle) * (*sim)->number_of_coders);
	if (!(*sim)->dongles)
		return (0);
	return (1);
}

int	init_queue_of_dongles(t_dongle **dongles, int number_of_coders)
{
	int	i;

	i = 0;
	while (i < number_of_coders)
	{
		(*dongles)[i].queue = malloc(sizeof(t_queue));
		if (!(*dongles)[i].queue)
			return (free_queues(dongles, i), 0);
		init_queue_with_default_values(&(*dongles)[i].queue, number_of_coders);
		i++;
	}
	return (1);
}
