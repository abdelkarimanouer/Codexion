/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 10:54:42 by aanouer           #+#    #+#             */
/*   Updated: 2026/04/20 08:38:23 by aanouer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_queue_empty(t_queue *queue)
{
	if (!queue || queue->number_of_tickets <= 0)
		return (1);
	else
		return (0);
}

void	free_queues(t_dongle **dongles, int i)
{
	int	j;

	j = 0;
	while (j < i)
	{
		free((*dongles)[j].queue->requests);
		free((*dongles)[j].queue);
		j++;
	}
}

unsigned long long	get_current_time(void)
{
	struct timeval		tv;
	unsigned long long	current_time;

	gettimeofday(&tv, NULL);
	current_time = (unsigned long long)((tv.tv_sec * 1000)
			+ (tv.tv_usec / 1000));
	return (current_time);
}
