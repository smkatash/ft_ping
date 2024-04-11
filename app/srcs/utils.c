#include "ft_ping.h"

unsigned short checksum(void *header, int len) {
    unsigned short *ptr = (unsigned short *)header;
    unsigned int sum = 0;
    unsigned short checksum;

    while (len > 1) {
        sum += *ptr++;
        len -= 2;
    }
    if (len == 1) {
        sum += *((unsigned char *)ptr);
    }
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    checksum = (unsigned short)~sum; 
    return checksum;
}

void    init_stats(t_ping *p) {
    p->seq = 0;
    p->interval = p->opts.wait ? (double)p->opts.wait : 1;
    p->rtt = 0;
    p->min = 0;
    p->max = 0;
    p->sum = 0;
    p->sum2 = 0;
    p->sent = 0;
    p->received = 0;
    p->cpid = (uint16_t)getpid();
}


int  valid_response(void *msg, t_ping *p) {
	struct iphdr	*iphdr;
    struct icmphdr  *icmp;

	iphdr = (struct iphdr *)msg;
	if (iphdr->protocol != IPPROTO_ICMP) {
        free_ping(p);
		log_error(WRONG_IP_PROTOCOL);
	}
	icmp = (struct icmphdr *)(msg + (iphdr->ihl * 4));
    if (icmp->type == ICMP_ECHOREPLY && ntohs(icmp->un.echo.id) == p->cpid) {
        return (1);
    } else if (icmp->type != ICMP_ECHOREPLY && p->opts.verbose && !p->opts.quiet) {
		log_icmp(icmp->type, icmp->code);
	}
	return (0);
}

void    free_ping(t_ping *p) {
    if (p->socket) {
        close(p->socket);
    }
    if (p->hostinfo.hostname) {
        free(p->hostinfo.hostname);
        p->hostinfo.hostname = NULL;
    }
    if (p->hostinfo.server_hostname) {
        free(p->hostinfo.server_hostname);
        p->hostinfo.server_hostname = NULL;
    }
}