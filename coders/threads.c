/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 09:21:58 by aanouer           #+#    #+#             */
/*   Updated: 2026/04/24 10:42:51 by aanouer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	rest_of_code(t_coder *coder)
{
	print_action(coder->sim, coder->id, "is debugging");
	my_sleep(coder->sim->time_to_debug, coder->sim);
	if (check_simulation_stop(coder->sim))
		return ;
	print_action(coder->sim, coder->id, "is refactoring");
	my_sleep(coder->sim->time_to_refactor, coder->sim);
	if (check_simulation_stop(coder->sim))
		return ;
	pthread_mutex_lock(&coder->lock_l_c_s);
	coder->last_compile_start = get_current_time();
	pthread_mutex_unlock(&coder->lock_l_c_s);
}

void	*coder_routine(void *arg)
{
	t_coder		*coder;
	t_dongle	*first;
	t_dongle	*second;

	coder = (t_coder *)arg;
	if (coder->id % 2 == 0)
		usleep(1000);
	while (1)
	{
		if (check_simulation_stop(coder->sim))
			break ;
		if (coder->left_dongle < coder->right_dongle)
		{
			first = coder->left_dongle;
			second = coder->right_dongle;
		}
		else
		{
			first = coder->right_dongle;
			second = coder->left_dongle;
		}
		if (!take_dongle(coder, first))
			break ;
		if (!take_dongle(coder, second))
		{
			release_dongle(coder, first);
			break ;
		}
		print_action(coder->sim, coder->id, "is compiling");
		my_sleep(coder->sim->time_to_compile, coder->sim);
		release_dongle(coder, first);
		release_dongle(coder, second);
		pthread_mutex_lock(&coder->lock_l_c_s);
		coder->last_compile_start = get_current_time();
		pthread_mutex_unlock(&coder->lock_l_c_s);
		coder->compile_count++;
		rest_of_code(coder);
		if (coder->compile_count >= coder->sim->number_of_compiles_required)
			break ;
	}
	return (NULL);
}

void	*monitor_routine(void *arg)
{
	t_simulation	*sim;
	long			i;
	t_coder			*coder;

	sim = (t_simulation *)arg;
	while (1)
	{
		if (check_simulation_stop(sim))
			return (NULL);
		i = 0;
		while (i < sim->number_of_coders)
		{
			coder = &sim->coders[i];
			pthread_mutex_lock(&coder->lock_l_c_s);
			if (get_current_time() - coder->last_compile_start > sim->time_to_burnout)
			{
				pthread_mutex_unlock(&coder->lock_l_c_s);
				pthread_mutex_lock(&sim->stop_mutex);
				sim->stop = 1;
				pthread_mutex_unlock(&sim->stop_mutex);
				print_action(sim, coder->id, "burned out");
				return (NULL);
			}
			pthread_mutex_unlock(&coder->lock_l_c_s);
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}

void	join_threads(t_simulation *sim)
{
	long	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		pthread_join(sim->coders[i].coder_thread, NULL);
		i++;
	}
	pthread_mutex_lock(&sim->stop_mutex);
	sim->stop = 1;
	pthread_mutex_unlock(&sim->stop_mutex);
	pthread_join(sim->monitor, NULL);
}

int	start_threads(t_simulation *sim)
{
	long i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		if (pthread_create(&sim->coders[i].coder_thread, NULL,
			coder_routine, &sim->coders[i]) != 0)
		{
			fprintf(stderr, "Error creating coder thread %lu\n", i);
			return (1);
		}
		i++;
	}
	if (pthread_create(&sim->monitor, NULL, monitor_routine, sim) != 0)
	{
		fprintf(stderr, "Error creating monitor thread\n");
		return (1);
	}
	return (0);
}
