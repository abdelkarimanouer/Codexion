/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   min_heap_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 18:11:24 by aanouer           #+#    #+#             */
/*   Updated: 2026/05/08 11:57:01 by aanouer          ###   ########.fr       */
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

void	bubble_up(t_queue *queue, int i, char *scheduler)
{
	int			the_winner;
	t_request	temp;

	while (i > 0)
	{
		the_winner = (i - 1) / 2;
		if (goes_first(queue->request[i], queue->request[the_winner],
				scheduler))
		{
			temp = queue->request[i];
			queue->request[i] = queue->request[the_winner];
			queue->request[the_winner] = temp;
		}
		else
			break ;
		i = the_winner;
	}
}

void	bubble_down(t_queue *queue, int i, char *scheduler)
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
			&& goes_first(queue->request[right_child],
				queue->request[left_child], scheduler))
			the_winner = right_child;
		if (!goes_first(queue->request[the_winner], queue->request[i],
				scheduler))
			break ;
		temp = queue->request[i];
		queue->request[i] = queue->request[the_winner];
		queue->request[the_winner] = temp;
		i = the_winner;
	}
}

t_request	*get_the_winner(t_queue *queue)
{
	if (!queue || queue->number_of_tickets <= 0 || !queue->request)
		return (NULL);
	return (&(queue->request[0]));
}

int	is_queue_empty(t_queue *queue)
{
	if (!queue || queue->number_of_tickets <= 0)
		return (1);
	return (0);
}
