#include "ft_ping.h"

void    log_error(char *err) {
    fprintf(stderr, "%s\n", err);
    exit(EXIT_FAILURE);
}


void    log_info(char *str) {
    fprintf(stdout, "%s\n", str);
}

void    log_header(t_ping *p) {
    fprintf(stdout, "PING %s (%s) 56(84) bytes of data.\n", p->hostinfo.hostname, p->hostinfo.ip_addr);
}

void    log_response(t_ping *p, t_response *resp, int bytes, uint16_t seq) {
    struct ip	*iphdr;
    uint16_t    ttl;
    double      time;
    
    iphdr = (struct ip *)resp->msg;
    ttl = (uint16_t)iphdr->ip_ttl;
    time = get_round_trip_time(p->start_time, p->end_time);
    if (ft_strcmp(p->hostinfo.hostname, p->hostinfo.ip_addr) == 0) {
        fprintf(stdout, "%lu bytes from %s: icmp_seq=%u ttl=%u time=%.2lf ms\n", bytes,
            p->hostinfo.hostname, ntohs(seq), ttl, time);
    } else {
        fprintf(stdout, "%lu bytes from %s (%s): icmp_seq=%u ttl=%u time=%.2lf ms\n", bytes,
            p->hostinfo.hostname, p->hostinfo.ip_addr, ntohs(seq), ttl, time);
    }
    save_time_stats(time);
}

void    log_stats(int sig) {
    (void)sig;
    int lost;
    double rtt;
    struct timeval  end;
    double  avg;

    timestamp(&end);
    rtt = get_round_trip_time(g_ping_stats.start, end);
    lost = 100 - ((g_ping_stats.received * 100) / g_ping_stats.sent);
    avg = g_ping_stats.total / g_ping_stats.sent;
    fprintf(stdout, "\n--- %s ping statistics ---\n", g_ping_stats.hostname);
    fprintf(stdout, "%d packets transmitted, %d received, %d%% packet loss, time %.0lfms\n", g_ping_stats.sent,
    g_ping_stats.received, lost, rtt);
    fprintf(stdout, "rtt min/avg/max/mdev = %.3lf/%.3lf/%.3lf/%.3lf ms", g_ping_stats.min,
    avg, g_ping_stats.max, mdev());
    exit(EXIT_SUCCESS);
}