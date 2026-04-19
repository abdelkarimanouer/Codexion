/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 10:54:42 by aanouer           #+#    #+#             */
/*   Updated: 2026/04/19 10:58:43 by aanouer          ###   ########.fr       */
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
