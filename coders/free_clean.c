/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_clean.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 13:50:06 by aanouer           #+#    #+#             */
/*   Updated: 2026/04/22 13:50:38 by aanouer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	free_dongles_and_queues(t_simulation *sim)
{
	unsigned long i;

	if (!sim->dongles)
		return ;
	for (i = 0; i < sim->number_of_coders; i++)
	{
		pthread_mutex_destroy(&sim->dongles[i].lock_dongle);
		pthread_cond_destroy(&sim->dongles[i].condition);
		if (sim->dongles[i].queue)
		{
			free(sim->dongles[i].queue->requests);
			free(sim->dongles[i].queue);
		}
	}
	free(sim->dongles);
}

static void	free_coders_and_mutexes(t_simulation *sim)
{
	unsigned long i;

	if (!sim->coders)
		return ;
	for (i = 0; i < sim->number_of_coders; i++)
		pthread_mutex_destroy(&sim->coders[i].lock_l_c_s);
	free(sim->coders);
}

void    free_clean_everything(t_simulation **sim)
{
	if (!sim || !*sim)
		return ;
	pthread_mutex_destroy(&(*sim)->stop_mutex);
	pthread_mutex_destroy(&(*sim)->print_mutex);
	pthread_mutex_destroy(&(*sim)->ticket_count_mutex);
	free_dongles_and_queues(*sim);
	free_coders_and_mutexes(*sim);
	free(*sim);
	*sim = NULL;
}
