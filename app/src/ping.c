#include "ft_ping.h"


static void    send_packet(t_ping *p) {
    unsigned char packet[ICMP_HDR_SIZE + ICMP_DATA_SIZE];
    size_t packetlen;
    struct icmphdr *hdr;

    packetlen = sizeof(packet);
    ft_memset(&packet, 0, packetlen);
    hdr = (struct icmphdr *)packet;
    hdr->type = ICMP_ECHO;
    hdr->code = 0;
    hdr->checksum = 0;
    hdr->un.echo.id = htons(p->cpid);
    hdr->un.echo.sequence = htons(++p->seq);
    hdr->checksum = checksum(packet, packetlen);
    if (gettimeofday(&(p->start_rtt), NULL) == -1) {
        log_error(strerror(errno));
    }
    if (sendto(p->socket, &packet, packetlen, 0,
            (const struct sockaddr *)&p->hostinfo.inet_addr,
            sizeof(p->hostinfo.inet_addr)) < 0) {
                log_error(strerror(errno));
    }
    ++p->sent;
}

static ssize_t receive_packet(void *msg, size_t msglen, t_ping *p) {
    struct msghdr   hdr;
    struct iovec    iov;
    ssize_t         ret;

    ft_memset(msg, 0, msglen);
    iov.iov_base = msg;
    iov.iov_len = msglen;
    hdr.msg_name = p->hostinfo.ip_addr;
	hdr.msg_iov = &(iov);
	hdr.msg_iovlen = 1;
    if ((ret = recvmsg(p->socket, &hdr, 0)) == -1) {
        //if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR) {
        if (errno == 11 || errno == EWOULDBLOCK  || errno == EINTR) {
            return (0);
        }
        log_error(strerror(errno));
    }

    if (gettimeofday(&(p->end_rtt), NULL) == -1) {
        log_error(strerror(errno));
    }
    if (!valid_response(msg, p)) {
        return (-1);
    }
    ++p->received;
    return ret;
}


void    ping(t_ping *p) {
    unsigned char   msg[MAX_PACKET_SIZE];
    ssize_t rbytes;

    send_packet(p);
    rbytes = receive_packet(msg, sizeof(msg), p);
    p->rtt = get_round_trip_time(p->start_rtt, p->end_rtt);
    if (rbytes == -1 && p->opts.verbose && !p->opts.quiet) {
        log_error_verbose(msg);
    } else if (rbytes > 0 && !p->opts.quiet) {
        log_response(p, msg, rbytes - IP_HDR_SIZE);
    }
    save_time_stats(p);
}