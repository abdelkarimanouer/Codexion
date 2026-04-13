/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 13:25:16 by aanouer           #+#    #+#             */
/*   Updated: 2026/04/13 13:24:01 by aanouer          ###   ########.fr       */
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

static void	wait_until_cooldown_over(t_dongle *dongle, long long cooldown_until)
{
	struct timespec	ts;

	ts.tv_sec = cooldown_until / 1000000LL;
	ts.tv_nsec = (cooldown_until % 1000000LL) * 1000LL;
	pthread_cond_timedwait(&dongle->condition, &dongle->mutex, &ts);
}

void	take_left_dongle(t_coder *coder)
{
	struct timeval	tv;
	long long		current_time;

	pthread_mutex_lock(&coder->left_dongle->mutex);
	gettimeofday(&tv, NULL);
	current_time = (long long)tv.tv_sec * 1000000LL + tv.tv_usec;
	if (coder->left_dongle->cooldown_until > current_time
		|| coder->left_dongle->is_available == 0)
		add_coder_to_queue(coder->left_dongle, coder, coder->sim->scheduler);
	while (coder->left_dongle->cooldown_until > current_time
		|| coder->left_dongle->is_available == 0)
	{
		if (is_stopped(coder->sim))
		{
			pthread_mutex_unlock(&coder->left_dongle->mutex);
			return ;
		}
		if (coder->left_dongle->is_available
			&& coder->left_dongle->cooldown_until > current_time)
			wait_until_cooldown_over(coder->left_dongle,
				coder->left_dongle->cooldown_until);
		else
			pthread_cond_wait(&coder->left_dongle->condition,
				&coder->left_dongle->mutex);
		gettimeofday(&tv, NULL);
		current_time = (long long)tv.tv_sec * 1000000LL + tv.tv_usec;
	}
	remove_coder_from_queue(coder->left_dongle, coder);
	if (is_stopped(coder->sim))
	{
		pthread_mutex_unlock(&coder->left_dongle->mutex);
		return ;
	}
	coder->left_dongle->is_available = 0;
	pthread_mutex_unlock(&coder->left_dongle->mutex);
	log_action(coder->sim, coder->id, "has taken a dongle");
}

void	take_right_dongle(t_coder *coder)
{
	struct timeval	tv;
	long long		current_time;

	pthread_mutex_lock(&coder->right_dongle->mutex);
	gettimeofday(&tv, NULL);
	current_time = (long long)tv.tv_sec * 1000000LL + tv.tv_usec;
	if (coder->right_dongle->cooldown_until > current_time
		|| coder->right_dongle->is_available == 0)
		add_coder_to_queue(coder->right_dongle, coder, coder->sim->scheduler);
	while (coder->right_dongle->cooldown_until > current_time
		|| coder->right_dongle->is_available == 0)
	{
		if (is_stopped(coder->sim))
		{
			pthread_mutex_unlock(&coder->right_dongle->mutex);
			return ;
		}
		if (coder->right_dongle->is_available
			&& coder->right_dongle->cooldown_until > current_time)
			wait_until_cooldown_over(coder->right_dongle,
				coder->right_dongle->cooldown_until);
		else
			pthread_cond_wait(&coder->right_dongle->condition,
				&coder->right_dongle->mutex);
		gettimeofday(&tv, NULL);
		current_time = (long long)tv.tv_sec * 1000000LL + tv.tv_usec;
	}
	remove_coder_from_queue(coder->right_dongle, coder);
	if (is_stopped(coder->sim))
	{
		pthread_mutex_unlock(&coder->right_dongle->mutex);
		return ;
	}
	coder->right_dongle->is_available = 0;
	pthread_mutex_unlock(&coder->right_dongle->mutex);
	log_action(coder->sim, coder->id, "has taken a dongle");
}

void	release_left_dongle(t_coder *coder)
{
	struct timeval	tv;
	long long		current_time;

	pthread_mutex_lock(&coder->left_dongle->mutex);
	gettimeofday(&tv, NULL);
	current_time = (long long)tv.tv_sec * 1000000LL + tv.tv_usec;
	coder->left_dongle->cooldown_until = (current_time
			+ (coder->sim->dongle_cooldown * 1000));
	coder->left_dongle->is_available = 1;
	pthread_cond_broadcast(&coder->left_dongle->condition);
	pthread_mutex_unlock(&coder->left_dongle->mutex);
}

void	release_right_dongle(t_coder *coder)
{
	struct timeval	tv;
	long long		current_time;

	pthread_mutex_lock(&coder->right_dongle->mutex);
	gettimeofday(&tv, NULL);
	current_time = (long long)tv.tv_sec * 1000000LL + tv.tv_usec;
	coder->right_dongle->cooldown_until = (current_time
			+ (coder->sim->dongle_cooldown * 1000));
	coder->right_dongle->is_available = 1;
	pthread_cond_broadcast(&coder->right_dongle->condition);
	pthread_mutex_unlock(&coder->right_dongle->mutex);
}
