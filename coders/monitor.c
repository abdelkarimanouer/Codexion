/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 11:32:03 by aanouer           #+#    #+#             */
/*   Updated: 2026/04/13 14:11:23 by aanouer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	wake_all_waiters(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		pthread_mutex_lock(&sim->dongles[i].mutex);
		pthread_cond_broadcast(&sim->dongles[i].condition);
		pthread_mutex_unlock(&sim->dongles[i].mutex);
		i++;
	}
	pthread_mutex_lock(&sim->turn_mutex);
	pthread_cond_broadcast(&sim->turn_cond);
	pthread_mutex_unlock(&sim->turn_mutex);
}

static long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((long long)tv.tv_sec * 1000000LL + tv.tv_usec);
}

static int	check_if_coders_compiled_enough(t_simulation *sim)
{
	int	i;

	if (sim->number_of_compiles_required <= 0)
		return (0);
	i = 0;
	pthread_mutex_lock(&sim->stop_mutex);
	while (i < sim->number_of_coders)
	{
		if (sim->coders[i].compile_count < sim->number_of_compiles_required)
		{
			pthread_mutex_unlock(&sim->stop_mutex);
			return (0);
		}
		i++;
	}
	sim->stop_simulation = 1;
	pthread_mutex_unlock(&sim->stop_mutex);
	wake_all_waiters(sim);
	return (1);
}

static int	check_burnout(t_simulation *sim)
{
	int			i;
	long long	current_time;

	i = 0;
	while (i < sim->number_of_coders)
	{
		current_time = get_time();
		pthread_mutex_lock(&sim->stop_mutex);
		if (sim->stop_simulation)
		{
			pthread_mutex_unlock(&sim->stop_mutex);
			return (1);
		}
		sim->t_s_comp = current_time - sim->coders[i].last_compile_time;
		if (sim->t_s_comp > (sim->time_to_burnout * 1000))
		{
			sim->stop_simulation = 1;
			pthread_mutex_unlock(&sim->stop_mutex);
			log_action(sim, sim->coders[i].id, "burned out");
			wake_all_waiters(sim);
			return (1);
		}
		pthread_mutex_unlock(&sim->stop_mutex);
		i++;
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_simulation	*sim;

	sim = (t_simulation *)arg;
	while (1)
	{
		pthread_mutex_lock(&sim->stop_mutex);
		if (sim->stop_simulation)
		{
			pthread_mutex_unlock(&sim->stop_mutex);
			break ;
		}
		pthread_mutex_unlock(&sim->stop_mutex);
		if (check_burnout(sim))
			break ;
		if (check_if_coders_compiled_enough(sim))
			break ;
		usleep(1000);
	}
	return (NULL);
}
