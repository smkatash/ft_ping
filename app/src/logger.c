#include "ft_ping.h"

void    log_error(const char *err) {
    dprintf(STDERR_FILENO, "ft_ping: %s\n", err);
    exit(EXIT_FAILURE);
}


void    log_info(const char *str) {
    dprintf(STDOUT_FILENO, "%s\n", str);
}

void    log_header(t_ping *p) {
    dprintf(STDOUT_FILENO, "PING %s (%s) 56(84) bytes of data.\n", 
        p->hostinfo.hostname, p->hostinfo.ip_addr);
}

void    log_response(t_ping *p, void *msg, int bytes) {
    struct iphdr	*ip;
    struct icmphdr  *icmp;
    uint16_t    ttl;
    uint16_t    seq;
    
    ip = (struct iphdr *)msg;
    ttl = (uint16_t)ip->ttl;
    icmp = (struct icmphdr *)(msg + (ip->ihl * 4));
    seq = icmp->un.echo.sequence;
    if (ft_strcmp(p->hostinfo.hostname, p->hostinfo.ip_addr) == 0) {
        dprintf(STDOUT_FILENO, "%d bytes from %s: icmp_seq=%u ttl=%u time=%.2lf ms\n", 
        bytes, p->hostinfo.hostname, ntohs(seq), ttl, p->rtt);
    } else {
        if (ft_strlen(p->hostinfo.server_hostname) > 0) {
            dprintf(STDOUT_FILENO, "%d bytes from %s (%s): icmp_seq=%u ttl=%u time=%.2lf ms\n", 
            bytes, p->hostinfo.server_hostname, p->hostinfo.ip_addr, ntohs(seq), ttl, p->rtt);
        } else {
            dprintf(STDOUT_FILENO, "%d bytes from %s (%s): icmp_seq=%u ttl=%u time=%.2lf ms\n", 
            bytes, p->hostinfo.hostname, p->hostinfo.ip_addr, ntohs(seq), ttl, p->rtt);
        }
    }
}

void    log_stats(t_ping *p) {
    int lost;
    double rtt_ping;
    struct timeval  end;
    double  avg;
    double  tvar;

    if (gettimeofday(&(end), NULL) == -1) {
        log_error(strerror(errno));
    }
    rtt_ping = get_round_trip_time(p->start, end);
    avg = p->sum / p->sent;
    if (p->sum < INT_MAX) {
		tvar = (p->sum2 - ((p->sum * p->sum) / p->sum)) / p->sum;
    } else {
		tvar = (p->sum2 / p->sum) - (avg * avg);
    }
    lost = 100 - ((p->received * 100) / p->sent);
    dprintf(STDOUT_FILENO, "\n--- %s ping statistics ---\n", p->hostinfo.hostname);
    dprintf(STDOUT_FILENO, "%d packets transmitted, %d received, %d%% packet loss, time %.0lfms\n", 
            p->sent, p->received, lost, rtt_ping);
    dprintf(STDOUT_FILENO, "rtt min/avg/max/mdev = %.3lf/%.3lf/%.3lf/%.3lf ms\n", 
            p->min,avg, p->max, double_sqrt(tvar));
    exit(EXIT_SUCCESS);
}


void log_help() {
	dprintf(STDOUT_FILENO, "Usage: ping [OPTION...] HOST ...\n"
	       "Send ICMP ECHO_REQUEST packets to network hosts.\n\n"
	       " Options:\n"
	       "  -?                 Display a help list.\n"
	       "  -v                 Verbose output.\n");
    exit(EXIT_SUCCESS);
}