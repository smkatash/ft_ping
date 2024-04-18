/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanykei <kanykei@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 15:06:35 by kanykei           #+#    #+#             */
/*   Updated: 2024/04/18 15:21:03 by kanykei          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void    save_time_stats(t_ping *p)
{
	if (p->min == 0 || p->min > p->rtt)
	{
		p->min = p->rtt;
	}
	if (p->max == 0 || p->max < p->rtt)
	{
		p->max = p->rtt;
	}
	
	p->sum += p->rtt;
	p->sum2 += p->rtt * p->rtt;
}

static double double_sqrt(double a)
{
	double x = a;
	double prev = NAN;

	while (x != prev)
	{
		prev = x;
		x = 0.5 * (x + a / x);
	}
	return (x);
}

double  mdev(t_ping *p, double avg)
{
	double	tvar;

	if (p->sum < INT_MAX)
	{
		tvar = (p->sum2 - ((p->sum * p->sum) / p->sum)) / p->sum;
	}
	else
	{
		tvar = (p->sum2 / p->sum) - (avg * avg);
	}
	return (double_sqrt(tvar));
}

void wsleep(t_ping *p)
{
	struct  timeval	start;
	struct  timeval	end;

	start = p->start_rtt;
	end.tv_sec = start.tv_sec + p->interval;
	end.tv_usec = start.tv_usec + ((p->interval - p->interval) * 1000000);
	while (timercmp(&(start), &end, <))
	{
		gettimeofday(&(start), NULL);
	}
}

inline double	get_round_trip_time(struct timeval start, struct timeval end)
{
	return ((double)(end.tv_sec - start.tv_sec) * 1000 +
			(double)(end.tv_usec - start.tv_usec) / 1000);
}
