/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 09:12:17 by aanouer           #+#    #+#             */
/*   Updated: 2026/04/28 10:38:32 by aanouer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	take_dongle_wait(t_coder *coder, t_dongle *dongle)
{
	while (dongle->is_available == 0
		|| is_queue_empty(dongle->queue)
		|| get_the_winner(dongle->queue)->coder_id != coder->id
		|| get_current_time() < dongle->cooldown_end)
	{
		pthread_mutex_unlock(&dongle->lock_dongle);
		if (check_simulation_stop(coder->sim))
		{
			pthread_mutex_lock(&dongle->lock_dongle);
			pop_request(dongle->queue, coder->sim->scheduler);
			pthread_mutex_unlock(&dongle->lock_dongle);
			return (0);
		}
		usleep(500);
		pthread_mutex_lock(&dongle->lock_dongle);
	}
	return (1);
}

int	take_dongle(t_coder *coder, t_dongle *dongle)
{
	t_request	req;

	req.coder_id = coder->id;
	pthread_mutex_lock(&coder->sim->ticket_count_mutex);
	req.ticket_number = coder->sim->ticket_count++;
	pthread_mutex_unlock(&coder->sim->ticket_count_mutex);
	pthread_mutex_lock(&coder->lock_l_c_s);
	req.deadline = coder->last_compile_start + coder->sim->time_to_burnout;
	pthread_mutex_unlock(&coder->lock_l_c_s);
	pthread_mutex_lock(&dongle->lock_dongle);
	push_request(dongle->queue, req, coder->sim->scheduler);
	if (!take_dongle_wait(coder, dongle))
		return (0);
	dongle->is_available = 0;
	pop_request(dongle->queue, coder->sim->scheduler);
	pthread_mutex_unlock(&dongle->lock_dongle);
	print_action(coder->sim, coder->id, "has taken a dongle");
	return (1);
}

void	release_dongle(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->lock_dongle);
	dongle->is_available = 1;
	dongle->cooldown_end = get_current_time() + coder->sim->dongle_cooldown;
	pthread_mutex_unlock(&dongle->lock_dongle);
}
