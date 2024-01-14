#include "ft_ping.h"

void    timestamp(struct timeval *tv) {
    gettimeofday(tv, NULL);
}

void    save_time_stats(double time) {
    if (g_ping_stats.min == 0 || g_ping_stats.min > time) {
        g_ping_stats.min = time;
    }
    if (g_ping_stats.max == 0 || g_ping_stats.max < time) {
        g_ping_stats.max = time;
    }
    g_ping_stats.total += time;
}

double  mdev() {
	// TODO
    return 0;
}

double    get_round_trip_time(struct timeval start, struct timeval end) {
    double sec_ms = ((double)end.tv_sec - (double)start.tv_sec) * 1000;
    double usec_ms = ((double)end.tv_usec - (double)start.tv_usec) / 1000;
    return sec_ms + usec_ms;
}

void wait_interval(t_ping *p) {
    struct  timeval start;
    struct  timeval end;

    start = p->start_time;
    end.tv_sec = start.tv_sec + p->interval;
    end.tv_usec = start.tv_usec + (double)((p->interval - (double)p->interval) * 1000000);
    while (timercmp(&(start), &end, <)) {
        timestamp(&(start));
    }
}


