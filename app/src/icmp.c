#include "ft_ping.h"

void log_icmp(int type, int code) {
	switch (type) {
	    case ICMP_DEST_UNREACH:
		    switch(code) {
            case ICMP_NET_UNREACH:
                dprintf(STDOUT_FILENO, "Destination Net Unreachable");
                break;
            case ICMP_HOST_UNREACH:
                dprintf(STDOUT_FILENO, "Destination Host Unreachable");
                break;
            case ICMP_PROT_UNREACH:
                dprintf(STDOUT_FILENO, "Destination Protocol Unreachable");
                break;
            case ICMP_PORT_UNREACH:
                dprintf(STDOUT_FILENO, "Destination Port Unreachable");
                break;
            case ICMP_FRAG_NEEDED:
                dprintf(STDOUT_FILENO, "Frag needed");
                break;
            case ICMP_SR_FAILED:
                dprintf(STDOUT_FILENO, "Source Route Failed");
                break;
            case ICMP_NET_UNKNOWN:
                dprintf(STDOUT_FILENO, "Destination Net Unknown");
                break;
            case ICMP_HOST_UNKNOWN:
                dprintf(STDOUT_FILENO, "Destination Host Unknown");
                break;
            case ICMP_HOST_ISOLATED:
                dprintf(STDOUT_FILENO, "Source Host Isolated");
                break;
            case ICMP_NET_ANO:
                dprintf(STDOUT_FILENO, "Destination Net Prohibited");
                break;
            case ICMP_HOST_ANO:
                dprintf(STDOUT_FILENO, "Destination Host Prohibited");
                break;
            case ICMP_NET_UNR_TOS:
                dprintf(STDOUT_FILENO, "Destination Net Unreachable for Type of Service\n");
                break;
            case ICMP_HOST_UNR_TOS:
                dprintf(STDOUT_FILENO, "Destination Host Unreachable for Type of Service");
                break;
            case ICMP_PKT_FILTERED:
                dprintf(STDOUT_FILENO, "Packet filtered");
                break;
            case ICMP_PREC_VIOLATION:
                dprintf(STDOUT_FILENO, "Precedence Violation");
                break;
            case ICMP_PREC_CUTOFF:
                dprintf(STDOUT_FILENO, "Precedence Cutoff");
                break;
            default:
                dprintf(STDOUT_FILENO, "Dest Unreachable, Bad Code: %d", code);
                break;
            }
		break;
	case ICMP_SOURCE_QUENCH:
		dprintf(STDOUT_FILENO, "Source Quench");
		break;
	case ICMP_REDIRECT:
		switch(code) {
		case ICMP_REDIR_NET:
			dprintf(STDOUT_FILENO, "Redirect Network");
			break;
		case ICMP_REDIR_HOST:
			dprintf(STDOUT_FILENO, "Redirect Host");
			break;
		case ICMP_REDIR_NETTOS:
			dprintf(STDOUT_FILENO, "Redirect Type of Service and Network");
			break;
		case ICMP_REDIR_HOSTTOS:
			dprintf(STDOUT_FILENO, "Redirect Type of Service and Host");
			break;
		default:
			dprintf(STDOUT_FILENO, "Redirect, Bad Code: %d", code);
			break;
		}
		break;
    case ICMP_TIME_EXCEEDED:
        switch(code) {
		case ICMP_EXC_TTL:
			dprintf(STDOUT_FILENO, "Time to live exceeded");
			break;
		case ICMP_EXC_FRAGTIME:
			dprintf(STDOUT_FILENO, "Frag reassembly time exceeded");
			break;
		default:
			dprintf(STDOUT_FILENO, "Time exceeded, Bad Code: %d", code);
			break;
		}
		break;
	}
    dprintf(STDOUT_FILENO, "\n");
}