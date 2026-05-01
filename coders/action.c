/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 08:42:07 by aanouer           #+#    #+#             */
/*   Updated: 2026/04/20 08:55:23 by aanouer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	print_action(t_simulation *sim, int id, char *action)
{
	int					is_stopped;
	unsigned long long	timestamp;

	pthread_mutex_lock(&sim->print_mutex);
	pthread_mutex_lock(&sim->stop_mutex);
	is_stopped = sim->stop;
	if (is_stopped)
	{
		pthread_mutex_unlock(&sim->stop_mutex);
		pthread_mutex_unlock(&sim->print_mutex);
		return ;
	}
	if (action[0] == 'b')
		sim->stop = 1;
	pthread_mutex_unlock(&sim->stop_mutex);
	timestamp = get_current_time() - sim->start_timestamp;
	printf("%llu %d %s\n", timestamp, id, action);
	pthread_mutex_unlock(&sim->print_mutex);
}
