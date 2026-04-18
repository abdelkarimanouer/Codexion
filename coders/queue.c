/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 18:11:24 by aanouer           #+#    #+#             */
/*   Updated: 2026/04/18 06:03:25 by aanouer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	goes_first(t_request a, t_request b, char *scheduler)
{
	if (strcmp(scheduler, "fifo") == 0)
	{
		if (a.ticket_number < b.ticket_number)
			return (1);
	}
	else if (strcmp(scheduler, "edf") == 0)
	{
		if (a.deadline < b.deadline)
			return (1);
		else if (a.deadline == b.deadline)
		{
			if (a.ticket_number < b.ticket_number)
				return (1);
		}
	}
	return (0);
}

void	push_request(t_queue *queue, t_request new_request, char *scheduler)
{
	int			i;
	int			the_winner;
	t_request	temp;

	i = queue->number_of_tickets;
	queue->requests[i] = new_request;
	queue->number_of_tickets++;
	i = queue->number_of_tickets;
	while (i > 0)
	{
		the_winner = (i - 1) / 2;
		if (i > 1 && goes_first(queue->requests[i], queue->requests[the_winner], scheduler))
		{
			temp = queue->requests[i];
			queue->requests[i] = queue->requests[the_winner];
			queue->requests[the_winner] = temp;
		}
		i--;
	}
}
