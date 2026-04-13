/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logging.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 11:18:32 by aanouer           #+#    #+#             */
/*   Updated: 2026/04/13 13:52:32 by aanouer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	log_action(t_simulation *sim, int coder_id, char *log)
{
	struct timeval	tv;
	long long		current_time;
	long long		timestamp;
	int				stopped;

	pthread_mutex_lock(&sim->stop_mutex);
	stopped = sim->stop_simulation;
	pthread_mutex_unlock(&sim->stop_mutex);
	if (stopped && strcmp(log, "burned out") != 0)
		return ;
	gettimeofday(&tv, NULL);
	current_time = (long long)tv.tv_sec * 1000000LL + tv.tv_usec;
	timestamp = (current_time - sim->start_time) / 1000;
	pthread_mutex_lock(&sim->print_mutex);
	printf("%lld %d %s\n", timestamp, coder_id, log);
	pthread_mutex_unlock(&sim->print_mutex);
}
