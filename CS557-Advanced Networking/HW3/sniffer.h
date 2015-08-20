/*
 * sniffer.h
 *
 *  Created on: Feb 3, 2014
 *  Author: Darshan Washimkar
 *
 */

#ifndef SNIFFER_H_
#define SNIFFER_H_
#define SNAP_LEN 1518  			 /* default snap length (maximum bytes per packet to capture) */
#define SIZE_ETHERNET 14		 /* size of Ethernet packet header */
typedef u_int32_t tcp_seq;

char *dev; /* Name of device to use*/
int time_interval; /* Time for which network sniffed */
int time_offset; /* Time offset */
pcap_t *handle; /* Sniffing session variable */
char *filter_exp; /* Filter expression */
struct bpf_program fp; /* Compilled expression */
char errbuf[PCAP_ERRBUF_SIZE]; /* Error buffer variable */
bpf_u_int32 addr; /* IP address of the device */
bpf_u_int32 mask; /* Subnet mask */
struct pcap_pkthdr header; /* The header that pcap gives us */
struct ether_header *eptr; /* Ethernet header*/
const u_char *packet; /* The actual packet */

time_t time_break, time_start; /* Variables to trigger and stop capture  */
int reading_from_file;

/* Ethernet header */
struct sniff_ethernet {
	u_char ether_dhost[ETHER_ADDR_LEN]; /* Destination host address */
	u_char ether_shost[ETHER_ADDR_LEN]; /* Source host address */
	u_short ether_type;
};

/* IP header */
struct sniff_ip {
	u_char ip_vhl; /* version << 4 | header length >> 2 */
	u_char ip_tos; /* type of service */
	u_short ip_len; /* total length */
	u_short ip_id; /* identification */
	u_short ip_off; /* fragment offset field */
#define IP_RF 0x8000			    /* reserved fragment flag */
#define IP_DF 0x4000				/* don't fragment flag */
#define IP_MF 0x2000			    /* more fragments flag */
#define IP_OFFMASK 0x1fff			/* mask for fragmenting bits */
	u_char ip_ttl; /* time to live */
	u_char ip_p; /* protocol */
	u_short ip_sum; /* checksum */
	struct in_addr ip_src, ip_dst; /* source and dest address */
};

#define IP_HL(ip)		(((ip)->ip_vhl) & 0x0f)
#define IP_V(ip)		(((ip)->ip_vhl) >> 4)

/* TCP header */
struct sniff_tcp {
	u_short th_sport; /* source port */
	u_short th_dport; /* destination port */
	tcp_seq th_seq; /* sequence number */
	tcp_seq th_ack; /* acknowledgement number */

	u_char th_offx2; /* data offset, rsvd */
#define TH_OFF(th)	(((th)->th_offx2 & 0xf0) >> 4)
	u_char th_flags;
#define TH_FIN 0x01
#define TH_SYN 0x02
#define TH_RST 0x04
#define TH_PUSH 0x08
#define TH_ACK 0x10
#define TH_URG 0x20
#define TH_ECE 0x40
#define TH_CWR 0x80
	u_short th_win; /* window */
	u_short th_sum; /* checksum */
	u_short th_urp; /* urgent pointer */
};

struct udphdr {
	u_short uh_sport; /* source port */
	u_short uh_dport; /* destination port */
	short uh_ulen; /* udp length */
	u_short uh_sum; /* udp checksum */
};

#endif /* SNIFFER_H_ */
