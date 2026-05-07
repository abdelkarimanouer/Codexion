/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   min_heap.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 18:11:24 by aanouer           #+#    #+#             */
/*   Updated: 2026/04/19 10:59:43 by aanouer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	push_request(t_queue *queue, t_request new_request, char *scheduler)
{
	int	i;

	i = queue->number_of_tickets;
	queue->requests[i] = new_request;
	queue->number_of_tickets++;
	bubble_up(queue, i, scheduler);
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
	bubble_down(queue, 0, scheduler);
}
