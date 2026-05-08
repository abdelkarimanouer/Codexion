/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_clean.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 13:50:06 by aanouer           #+#    #+#             */
/*   Updated: 2026/05/08 11:57:01 by aanouer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	free_dongles_and_queues(t_simulation *sim)
{
	long	i;

	if (!sim->dongles)
		return ;
	i = 0;
	while (i < sim->number_of_coders)
	{
		pthread_mutex_destroy(&sim->dongles[i].lock_dongle);
		pthread_cond_destroy(&sim->dongles[i].cond_dongle);
		if (sim->dongles[i].queue)
		{
			free(sim->dongles[i].queue->request);
			free(sim->dongles[i].queue);
		}
		i++;
	}
	free(sim->dongles);
}

static void	free_coders_and_mutexes(t_simulation *sim)
{
	long	i;

	if (!sim->coders)
		return ;
	i = 0;
	while (i < sim->number_of_coders)
	{
		pthread_mutex_destroy(&sim->coders[i].lock_l_c_s);
		i++;
	}
	free(sim->coders);
}

void	free_clean_everything(t_simulation **sim)
{
	if (!sim || !*sim)
		return ;
	pthread_mutex_destroy(&(*sim)->stop_mutex);
	pthread_mutex_destroy(&(*sim)->ticket_count_mutex);
	pthread_mutex_destroy(&(*sim)->sync_mutex);
	pthread_cond_destroy(&(*sim)->sync_cond);
	free_dongles_and_queues(*sim);
	free_coders_and_mutexes(*sim);
	free(*sim);
	*sim = NULL;
}
