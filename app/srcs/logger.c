#include "ft_ping.h"

void    log_error(const char *err) {
    dprintf(STDERR_FILENO, "ft_ping: %s\n", err);
    exit(EXIT_FAILURE);
}


void    log_info(const char *str) {
    dprintf(STDOUT_FILENO, "%s\n", str);
}

void    log_header(t_ping *p) {
    dprintf(STDOUT_FILENO, "PING %s (%s) %d(%d) bytes of data", 
        p->hostinfo.hostname, p->hostinfo.ip_addr, ICMP_DATA_SIZE, TOTAL_PACKET_SIZE);
    if (p->opts.verbose) {
        dprintf(STDOUT_FILENO, ", id 0x%04x = %d", (int)p->cpid, (int)p->cpid);
    }
    dprintf(STDOUT_FILENO, "\n");
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
    dprintf(STDOUT_FILENO, "%d bytes from %s: icmp_seq=%u ttl=%u time=%.2lf ms\n", 
    bytes, p->hostinfo.ip_addr, ntohs(seq), ttl, p->rtt);
}

void    log_stats(t_ping *p) {
    int lost;
    double  avg;

    lost = 100 - ((p->received * 100) / p->sent);
    dprintf(STDOUT_FILENO, "\n--- %s ping statistics ---\n", p->hostinfo.hostname);
    dprintf(STDOUT_FILENO, "%d packets transmitted, %d received, %d%% packet loss\n", 
            p->sent, p->received, lost);
    if (lost != 100) {
        avg = p->sum / p->sent;
        dprintf(STDOUT_FILENO, "round-trip min/avg/max/stddev = %.3lf/%.3lf/%.3lf/%.3lf ms\n", 
                p->min,avg, p->max, mdev(p, avg));
    }
}


void log_help() {
	dprintf(STDOUT_FILENO, "Usage: ft_ping [OPTION...] HOST ...\n"
	       "Send ICMP ECHO_REQUEST packets to network hosts.\n\n"
	       " Options valid for all request types:\n"
	       "  -?                 give this help list\n"
	       "  -v                 Verbose output.\n"
	       "  -q                 quiet output\n"
	       "  -c NUMBER          stop after sending NUMBER packets\n"
	       "  -i NUMBER          wait NUMBER seconds between sending each packet\n");
    exit(EXIT_SUCCESS);
}

void log_error_verbose(void *msg) {
    struct iphdr	*ip;
    struct icmphdr  *icmp;
    unsigned char   *buff;
    char            src[INET_ADDRSTRLEN]; 
    char            dest[INET_ADDRSTRLEN]; 

    ip = (struct iphdr *)msg;
    icmp = (struct icmphdr *)(msg + (ip->ihl * 4));
    buff = (unsigned char *)ip;
    dprintf(STDOUT_FILENO, "IP Hdr Dump:\n");
	for (size_t i = 0; i < sizeof(struct iphdr); i += 2) {
		dprintf(STDOUT_FILENO, " %02x%02x", *buff, *(buff + 1));
		buff += 2;
	}
    inet_ntop(AF_INET, &ip->saddr, src, sizeof(src));
    inet_ntop(AF_INET, &ip->daddr, dest, sizeof(dest));
    dprintf(STDOUT_FILENO,"\nVr HL TOS  Len   ID Flg  off TTL Pro  cks      Src    Dst    Data\n");
    dprintf(STDOUT_FILENO," %x  %x  %02x %04x %04x   %x %04x  %02x  %02x %04x ",
       ip->version, ip->ihl, ip->tos, ntohs(ip->tot_len),
       ntohs(ip->id), ntohs(ip->frag_off) >> 13,
       ntohs(ip->frag_off) & 0x1FFF, ip->ttl, ip->protocol,
       ntohs(ip->check));
    dprintf(STDOUT_FILENO,"%s  %s\n", src, dest);
    dprintf(STDOUT_FILENO,"ICMP: type %x, code %x, size %zu, id %#04x, seq 0x%04x\n",
       icmp->type, icmp->code, ICMP_DATA_SIZE + sizeof(*icmp),
       icmp->un.echo.id, icmp->un.echo.sequence);
}

