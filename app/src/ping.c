#include "ft_ping.h"

static unsigned short checksum(void *header, int len) {
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

    // right shift 32-bit by 16-bits && extract lower 16-bits of sum (0xFFFF hex for 16-1s)
    sum = (sum >> 16) + (sum & 0xFFFF);
    // add the carry from hight 16 bits to lower 16 bits of sum
    sum += (sum >> 16);
    checksum = ~sum;
    return checksum;
}

static void    fill_packet_header(t_packet *pckt, t_ping *p) {
    ft_memset(pckt, 0, sizeof(t_packet));
    pckt->icmp_header.icmp_type = ICMP_ECHO;
    pckt->icmp_header.icmp_code = 0;
    pckt->icmp_header.icmp_cksum = 0;
    p->cpid = (uint16_t)getpid();
    pckt->icmp_header.icmp_id = htons(p->cpid);
    pckt->icmp_header.icmp_seq = htons(p->seq++);
    pckt->icmp_header.icmp_cksum = checksum(pckt, sizeof(*pckt));
}

static void fill_response_header(t_response *resp, t_ping *p) {

    ft_memset(resp, 0, sizeof(t_response));
    resp->iov.iov_base = resp->msg;
    resp->iov.iov_len = sizeof(resp->msg);
    resp->msg_header.msg_name = p->hostinfo.ip_addr;
	resp->msg_header.msg_iov = &(resp->iov);
	resp->msg_header.msg_iovlen = 1;
	resp->msg_header.msg_control = 0;
	resp->msg_header.msg_controllen = 0;
}


static int    check_reply(t_response *resp, t_ping *p) {
	struct ip	*iphdr;
    struct icmp *icmp;

	iphdr = (struct ip *)resp->msg;
	if (iphdr->ip_p != IPPROTO_ICMP) {
		return (1);
	}
	icmp = (struct icmp *)(resp->msg + (iphdr->ip_hl << 2));
    if (icmp->icmp_type == ICMP_ECHOREPLY && ntohs(icmp->icmp_id) == p->cpid ) {
        return (3);
    } else if (icmp->icmp_type == ICMP_TIME_EXCEEDED && icmp->icmp_code == 0) {
		return (2);
	}
	return (4);
}


void    ping(t_ping *p) {
    t_packet pckt;
    t_response resp;
    int ret;
    int check;

    fill_packet_header(&pckt, p);
    fill_response_header(&resp, p);
    timestamp(&(p->start_time));
    ret = sendto(p->raw_socket, &pckt, sizeof(t_packet), 0, (const struct sockaddr *)&p->hostinfo.inet_addr, sizeof(p->hostinfo.inet_addr));
    if (ret > 0) {
        g_ping_stats.hostname = p->hostinfo.hostname;
        g_ping_stats.sent++;
        ret = recvmsg(p->raw_socket, &(resp.msg_header), 0);
        if (ret > 0) {
            timestamp(&(p->end_time));
            check = check_reply(&resp, p);
            if (check == 3) {
                g_ping_stats.received++;
                log_response(p, &resp, ret - sizeof(struct ip), pckt.icmp_header.icmp_seq);
            }
        }
    }
    wait_interval(p);
}