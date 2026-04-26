/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 09:21:58 by aanouer           #+#    #+#             */
/*   Updated: 2026/04/26 09:01:46 by aanouer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	join_threads(t_simulation *sim)
{
	long	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		pthread_join(sim->coders[i].coder_thread, NULL);
		i++;
	}
	pthread_mutex_lock(&sim->stop_mutex);
	sim->stop = 1;
	pthread_mutex_unlock(&sim->stop_mutex);
	pthread_join(sim->monitor, NULL);
}

static int	handle_coder_thread_fail(t_simulation *sim, long i)
{
	pthread_mutex_lock(&sim->stop_mutex);
	sim->stop = 1;
	pthread_mutex_unlock(&sim->stop_mutex);
	while (--i >= 0)
		pthread_join(sim->coders[i].coder_thread, NULL);
	return (1);
}

int	start_threads(t_simulation *sim)
{
	long	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		if (pthread_create(&sim->coders[i].coder_thread, NULL,
				coder_routine, &sim->coders[i]) != 0)
			return (handle_coder_thread_fail(sim, i));
		i++;
	}
	if (pthread_create(&sim->monitor, NULL, monitor_routine, sim) != 0)
	{
		pthread_mutex_lock(&sim->stop_mutex);
		sim->stop = 1;
		pthread_mutex_unlock(&sim->stop_mutex);
		i = 0;
		while (i < sim->number_of_coders)
		{
			pthread_join(sim->coders[i].coder_thread, NULL);
			i++;
		}
		return (1);
	}
	return (0);
}
