/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 20:59:35 by aanouer           #+#    #+#             */
/*   Updated: 2026/04/10 21:57:08 by aanouer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	add_coder_to_queue(t_dongle *dongle, t_coder *coder, char *scheduler)
{
	int	i;
	int	j;

	if (strcmp(scheduler, "fifo") == 0)
	{
		dongle->waiting_queue[dongle->waiting_count] = coder;
		dongle->waiting_count++;
	}
	else if (strcmp(scheduler, "edf") == 0)
	{
		i = 0;
		while (i < dongle->waiting_count && 
			dongle->waiting_queue[i]->deadline < coder->deadline)
			i++;
		j = dongle->waiting_count;
		while (j > i)
		{
			dongle->waiting_queue[j] = dongle->waiting_queue[j - 1];
			j--;
		}
		dongle->waiting_queue[i] = coder;
		dongle->waiting_count++;
	}
	else
		return ;
}
