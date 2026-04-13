/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 14:53:31 by aanouer           #+#    #+#             */
/*   Updated: 2026/04/13 15:18:29 by aanouer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	is_stopped(t_simulation *sim)
{
	int	stopped;

	pthread_mutex_lock(&sim->stop_mutex);
	stopped = sim->stop_simulation;
	pthread_mutex_unlock(&sim->stop_mutex);
	return (stopped);
}

int	wait_my_turn_or_stop(t_coder *coder)
{
	t_simulation	*sim;

	sim = coder->sim;
	pthread_mutex_lock(&sim->turn_mutex);
	while (!is_stopped(sim) && sim->turn_id != coder->id)
		pthread_cond_wait(&sim->turn_cond, &sim->turn_mutex);
	if (is_stopped(sim))
	{
		pthread_mutex_unlock(&sim->turn_mutex);
		return (0);
	}
	pthread_mutex_unlock(&sim->turn_mutex);
	return (1);
}

void	wake_all_waiters(t_simulation *sim)
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

void	wait_until_cooldown_over(t_dongle *dongle, long long cooldown_until)
{
	struct timespec	ts;

	ts.tv_sec = cooldown_until / 1000000LL;
	ts.tv_nsec = (cooldown_until % 1000000LL) * 1000LL;
	pthread_cond_timedwait(&dongle->condition, &dongle->mutex, &ts);
}

void	take_dongle(t_coder *coder, t_dongle *dongle)
{
	struct timeval	tv;
	long long		current_time;

	pthread_mutex_lock(&dongle->mutex);
	gettimeofday(&tv, NULL);
	current_time = (long long)tv.tv_sec * 1000000LL + tv.tv_usec;
	if (dongle->cooldown_until > current_time || dongle->is_available == 0)
		add_coder_to_queue(dongle, coder, coder->sim->scheduler);
	while (dongle->cooldown_until > current_time || dongle->is_available == 0)
	{
		if (is_stopped(coder->sim))
			return (pthread_mutex_unlock(&dongle->mutex), (void)0);
		if (dongle->is_available && dongle->cooldown_until > current_time)
			wait_until_cooldown_over(dongle, dongle->cooldown_until);
		else
			pthread_cond_wait(&dongle->condition, &dongle->mutex);
		gettimeofday(&tv, NULL);
		current_time = (long long)tv.tv_sec * 1000000LL + tv.tv_usec;
	}
	remove_coder_from_queue(dongle, coder);
	if (is_stopped(coder->sim))
		return (pthread_mutex_unlock(&dongle->mutex), (void)0);
	dongle->is_available = 0;
	pthread_mutex_unlock(&dongle->mutex);
	log_action(coder->sim, coder->id, "has taken a dongle");
}
