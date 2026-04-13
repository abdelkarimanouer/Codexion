/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 13:25:16 by aanouer           #+#    #+#             */
/*   Updated: 2026/04/13 15:08:04 by aanouer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	take_left_dongle(t_coder *coder)
{
	take_dongle(coder, coder->left_dongle);
}

void	take_right_dongle(t_coder *coder)
{
	take_dongle(coder, coder->right_dongle);
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
