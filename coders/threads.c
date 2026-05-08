/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 09:21:58 by aanouer           #+#    #+#             */
/*   Updated: 2026/05/08 12:09:59 by aanouer          ###   ########.fr       */
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
	pthread_mutex_lock(&sim->stop_sim_mutex);
	sim->stop_sim = 1;
	pthread_mutex_unlock(&sim->stop_sim_mutex);
	pthread_join(sim->monitor, NULL);
}

static void	handle_coder_thread_fail(t_simulation *sim, long i)
{
	pthread_mutex_lock(&sim->threads_ready_mutex);
	sim->threads_ready = -1;
	pthread_cond_broadcast(&sim->threads_ready_cond);
	pthread_mutex_unlock(&sim->threads_ready_mutex);
	pthread_mutex_lock(&sim->stop_sim_mutex);
	sim->stop_sim = 1;
	pthread_mutex_unlock(&sim->stop_sim_mutex);
	while (--i >= 0)
		pthread_join(sim->coders[i].coder_thread, NULL);
}

static int	handle_monitor_fail(t_simulation *sim)
{
	long	i;

	pthread_mutex_lock(&sim->threads_ready_mutex);
	sim->threads_ready = -1;
	pthread_cond_broadcast(&sim->threads_ready_cond);
	pthread_mutex_unlock(&sim->threads_ready_mutex);
	pthread_mutex_lock(&sim->stop_sim_mutex);
	sim->stop_sim = 1;
	pthread_mutex_unlock(&sim->stop_sim_mutex);
	i = 0;
	while (i < sim->number_of_coders)
	{
		pthread_join(sim->coders[i].coder_thread, NULL);
		i++;
	}
	return (1);
}

static void	start_simulation(t_simulation *sim)
{
	long	i;

	pthread_mutex_lock(&sim->threads_ready_mutex);
	sim->threads_ready = 1;
	sim->start_timestamp = get_current_time();
	while (get_current_time() == sim->start_timestamp)
		usleep(50);
	sim->start_timestamp = get_current_time();
	i = 0;
	while (i < sim->number_of_coders)
	{
		pthread_mutex_lock(&sim->coders[i].lock_l_c_s);
		sim->coders[i].last_compile_start = sim->start_timestamp;
		pthread_mutex_unlock(&sim->coders[i].lock_l_c_s);
		i++;
	}
	pthread_mutex_lock(&sim->stop_sim_mutex);
	sim->stop_sim = 0;
	pthread_mutex_unlock(&sim->stop_sim_mutex);
	pthread_cond_broadcast(&sim->threads_ready_cond);
	pthread_mutex_unlock(&sim->threads_ready_mutex);
}

int	start_threads(t_simulation *sim)
{
	long	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		if (pthread_create(&sim->coders[i].coder_thread, NULL, coder_routine,
				&sim->coders[i]) != 0)
			return (handle_coder_thread_fail(sim, i), 1);
		i++;
	}
	if (pthread_create(&sim->monitor, NULL, monitor_routine, sim) != 0)
		return (handle_monitor_fail(sim));
	start_simulation(sim);
	return (0);
}
