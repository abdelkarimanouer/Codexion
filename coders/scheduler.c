/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 20:59:35 by aanouer           #+#    #+#             */
/*   Updated: 2026/04/10 21:50:38 by aanouer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	add_coder_to_queue(t_dongle *dongle, t_coder *coder, char *scheduler)
{
	int	i;

	if (strcmp(scheduler, "fifo") == 0)
	{
		dongle->waiting_queue[dongle->waiting_count] = coder;
		dongle->waiting_count++;
	}
	else if (strcmp(scheduler, "edf") == 0)
	{
		i = 0;
		while (i < dongle->waiting_count)
		{
			if (dongle->waiting_queue[i]->deadline < coder->deadline)
				break;
			i++;
		}
		
	}
	else
		return ;
}
