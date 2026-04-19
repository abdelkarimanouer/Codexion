/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 18:11:24 by aanouer           #+#    #+#             */
/*   Updated: 2026/04/19 10:59:43 by aanouer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	goes_first(t_request a, t_request b, char *scheduler)
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
	while (i > 0)
	{
		the_winner = (i - 1) / 2;
		if (goes_first(queue->requests[i],
				queue->requests[the_winner], scheduler))
		{
			temp = queue->requests[i];
			queue->requests[i] = queue->requests[the_winner];
			queue->requests[the_winner] = temp;
		}
		else
			break ;
		i = the_winner;
	}
}

static void	reorder_queue(t_queue *queue, int i, char *scheduler)
{
	int			left_child;
	int			right_child;
	int			the_winner;
	t_request	temp;

	while ((2 * i) + 1 < queue->number_of_tickets)
	{
		left_child = (2 * i) + 1;
		right_child = (2 * i) + 2;
		the_winner = left_child;
		if (right_child < queue->number_of_tickets
			&& goes_first(queue->requests[right_child],
				queue->requests[left_child], scheduler))
			the_winner = right_child;
		if (!goes_first(queue->requests[the_winner],
				queue->requests[i], scheduler))
			break ;
		temp = queue->requests[i];
		queue->requests[i] = queue->requests[the_winner];
		queue->requests[the_winner] = temp;
		i = the_winner;
	}
}

void	pop_request(t_queue *queue, char *scheduler)
{
	t_request	temp;
	int			i;

	if (queue->number_of_tickets <= 0)
		return ;
	queue->number_of_tickets--;
	i = queue->number_of_tickets;
	temp = queue->requests[i];
	queue->requests[i] = queue->requests[0];
	queue->requests[0] = temp;
	reorder_queue(queue, 0, scheduler);
}

t_request	*get_the_winner(t_queue *queue)
{
	if (!queue || queue->number_of_tickets <= 0 || !queue->requests)
		return (NULL);
	return (&(queue->requests[0]));
}
