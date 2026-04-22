/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 09:12:17 by aanouer           #+#    #+#             */
/*   Updated: 2026/04/22 14:10:00 by aanouer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	take_dongle(t_coder *coder, t_dongle *dongle)
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
	while (dongle->is_available == 0
		|| !get_the_winner(dongle->queue)
		|| get_the_winner(dongle->queue)->coder_id != coder->id
		|| get_current_time() < dongle->cooldown_end
	)
		pthread_cond_wait(&dongle->condition, &dongle->lock_dongle);
	dongle->is_available = 0;
	pop_request(dongle->queue, coder->sim->scheduler);
	pthread_mutex_unlock(&dongle->lock_dongle);
	print_action(coder->sim, coder->id, "has taken a dongle");
}

void	release_dongle(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->lock_dongle);
	dongle->is_available = 1;
	dongle->cooldown_end = get_current_time() + coder->sim->dongle_cooldown;
	pthread_cond_broadcast(&dongle->condition);
	pthread_mutex_unlock(&dongle->lock_dongle);
}
