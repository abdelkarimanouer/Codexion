/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 18:11:24 by aanouer           #+#    #+#             */
/*   Updated: 2026/04/17 18:15:37 by aanouer          ###   ########.fr       */
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
