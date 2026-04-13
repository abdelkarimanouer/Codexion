/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 11:32:03 by aanouer           #+#    #+#             */
/*   Updated: 2026/04/13 13:37:11 by aanouer          ###   ########.fr       */
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
}

static void	check_if_coders_compiled_enough(t_simulation *sim, int *i)
{
	while (*i < sim->number_of_coders)
	{
		pthread_mutex_lock(&sim->stop_mutex);
		if (sim->coders[*i].compile_count
			< sim->number_of_compiles_required)
		{
			pthread_mutex_unlock(&sim->stop_mutex);
			break ;
		}
		pthread_mutex_unlock(&sim->stop_mutex);
		(*i)++;
	}
	if (*i == sim->number_of_coders)
	{
		pthread_mutex_lock(&sim->stop_mutex);
		sim->stop_simulation = 1;
		pthread_mutex_unlock(&sim->stop_mutex);
		wake_all_waiters(sim);
	}
}

static long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((long long)tv.tv_sec * 1000000LL + tv.tv_usec);
}

static void	check_burnout(t_simulation *sim, int *i)
{
	long long	current_time;

	while (*i < sim->number_of_coders)
	{
		current_time = get_time();
		pthread_mutex_lock(&sim->stop_mutex);
		sim->t_s_comp = current_time - sim->coders[*i].last_compile_time;
		pthread_mutex_unlock(&sim->stop_mutex);
		if (sim->t_s_comp > (sim->time_to_burnout * 1000))
		{
			log_action(sim, sim->coders[*i].id, "burned out");
			pthread_mutex_lock(&sim->stop_mutex);
			sim->stop_simulation = 1;
			pthread_mutex_unlock(&sim->stop_mutex);
			wake_all_waiters(sim);
			break ;
		}
		(*i)++;
	}
}

void	*monitor_routine(void *arg)
{
	t_simulation	*sim;
	int				i;

	sim = (t_simulation *)arg;
	pthread_mutex_lock(&sim->stop_mutex);
	while (sim->stop_simulation == 0)
	{
		pthread_mutex_unlock(&sim->stop_mutex);
		i = 0;
		check_burnout(sim, &i);
		i = 0;
		check_if_coders_compiled_enough(sim, &i);
		usleep(1000);
		pthread_mutex_lock(&sim->stop_mutex);
	}
	pthread_mutex_unlock(&sim->stop_mutex);
	return (NULL);
}
