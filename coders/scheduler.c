/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 20:59:35 by aanouer           #+#    #+#             */
/*   Updated: 2026/04/13 13:35:44 by aanouer          ###   ########.fr       */
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
		while (i < dongle->waiting_count
			&& dongle->waiting_queue[i]->deadline < coder->deadline)
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

t_coder	*get_next_coder_from_queue(t_dongle *dongle)
{
	t_coder	*coder;
	int		i;

	if (dongle->waiting_count == 0)
		return (NULL);
	coder = dongle->waiting_queue[0];
	i = 0;
	while (i < dongle->waiting_count - 1)
	{
		dongle->waiting_queue[i] = dongle->waiting_queue[i + 1];
		i++;
	}
	dongle->waiting_queue[dongle->waiting_count - 1] = NULL;
	dongle->waiting_count--;
	return (coder);
}

void	remove_coder_from_queue(t_dongle *dongle, t_coder *coder)
{
	int	i;
	int	j;

	i = 0;
	while (i < dongle->waiting_count)
	{
		if (dongle->waiting_queue[i] == coder)
			break ;
		i++;
	}
	if (i >= dongle->waiting_count)
		return ;
	j = i;
	while (j < dongle->waiting_count - 1)
	{
		dongle->waiting_queue[j] = dongle->waiting_queue[j + 1];
		j++;
	}
	dongle->waiting_queue[dongle->waiting_count - 1] = NULL;
	dongle->waiting_count--;
}
