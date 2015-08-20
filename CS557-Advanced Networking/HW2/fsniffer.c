#include <stdio.h>
#include <pcap.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/ethernet.h>
#include <time.h>
#include <flow.h>
#include <fsniffer.h>
#include <ip_icmp.h>



char *dev;            			 /* Name of device to use*/
int time_interval=0;    			 /* Time for which network sniffed */
int time_offset;      			 /* Time offset */
int num;
int sec;
struct flow *head=NULL;



void packet_received(u_char *args, const struct pcap_pkthdr *header, const u_char *packet)
{
	static int count=1;
	static int no_of_call=1;
	/* declare pointers to packet headers */
	const struct sniff_ethernet *ethernet;  /* The ethernet header [1] */
	const struct sniff_ip *ip;              /* The IP header  */
	const struct sniff_tcp *tcp;            /* The TCP header */
	const struct udphdr *udp;               /* The UDP header */
	const struct icmphdr *icmp;				/* The ICMP header */
	const char *payload;                    /* Packet payload */
	int size_ip;
	int size_tcp;
	int size_payload;
	struct timeval pcktime;
	char *prtc;
	char saddr[16];
	char daddr[16];
	uint16_t sport;
	uint16_t dport;
	bpf_u_int32 bytes;
	char state[10];
	char dir[5];

	/* While reading from file */

	if(no_of_call==1 && reading_from_file==1)
	{
		no_of_call++;
		time_break=(const time_t)(header->ts.tv_sec)+(const time_t)time_interval+(const time_t)time_offset;
		time_start=(const time_t)(header->ts.tv_sec)+(const time_t)time_offset;
//		printf("%s", ctime(&time_break));
	}

	/* While reading from file for offset manipulation */

	if(((difftime(time_start, (header->ts.tv_sec))) > 0.0) && (reading_from_file==1) && ((time_interval > 0) || (time_offset > 0))){
			return;
	}

	/* While reading from file, to terminate capturing loop */

	if(reading_from_file==1 && ((difftime(time_break, (header->ts.tv_sec))) < 0.0) && time_interval>0){

			pcap_breakloop(handle);
			return;
	}

	/* While live capturing, to terminate capturing loop */

	else if (time_break<=time(0) && reading_from_file==0 && time_interval > 0){
			pcap_breakloop(handle);
			return;
	}


	count++;

	/* Print time stamp */

//	printf(" Received @: %s",ctime((const time_t*)&header->ts.tv_sec));

	ethernet = (struct sniff_ethernet*)(packet);

	/* define/compute ip header offset */
	ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);

	/* Print Packet length */
//	printf(" Packet len: %d\n",header->caplen);

	/* define/compute ip header offset */
	ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);
	size_ip = IP_HL(ip)*4;
	if (size_ip < 20) {
//		printf("   ** Invalid IP header **\n");
		return;
	}


	pcktime=header->ts;
	strcpy(saddr,inet_ntoa(ip->ip_src));
	strcpy(daddr,inet_ntoa(ip->ip_dst));
	bytes=header->caplen;
	head = ifTimedOut(head,sec,pcktime);

	/* determine protocol */
		switch(ip->ip_p) {
			case IPPROTO_TCP:
			{
				tcp = (struct sniff_tcp*)(packet + SIZE_ETHERNET + size_ip);
				prtc="TCP";
				sport=(int)ntohs(tcp->th_sport);
				dport=(int)ntohs(tcp->th_dport);

				/*
				 * Printing Flags
				 */
				if(tcp->th_flags & TH_FIN){
					strcpy(state,"FIN");
					if(tcp->th_flags & TH_ACK){
						strcpy(state,"FINACK");
					}
					head=checkIfNewFlow (pcktime, prtc, saddr, daddr, sport, dport,"->", bytes, state, head);
					return;
				}

				else if(tcp->th_flags & TH_SYN){
					strcpy(state,"SYN");

					if(tcp->th_flags & TH_ACK){
						strcpy(state,"SYNACK");
					}
				}
				else if(tcp->th_flags & TH_RST){
					strcpy(state,"RST");
				}
				else if(tcp->th_flags & TH_ACK){
					strcpy(state,"ACK");
				}
				else {
					strcpy(state,"EST");
				}

				head=checkIfNewFlow (pcktime, prtc, saddr, daddr, sport, dport,"->", bytes, state, head);
				return;
			}
			case IPPROTO_UDP:
			{
				udp = (struct udphdr*)(packet + SIZE_ETHERNET + size_ip);
				prtc="UDP";
				sport=(int)ntohs(udp->uh_sport);
				dport=(int)ntohs(udp->uh_dport);
				strcpy(state,"");
				head=checkIfNewFlow (pcktime, prtc, saddr, daddr, sport, dport,"->", bytes, state, head);
				return;
			}

			case IPPROTO_ICMP:
			{
				icmp = (struct icmphdr*)(packet + SIZE_ETHERNET + size_ip);
				prtc="ICMP";
				sport=0;
				dport=0;
				sprintf(state, "%d", icmp->type);

				if(!(strcmp(state,"0")))
				{
					strcpy(state,"ECORLY");
				}
				else if(!(strcmp(state,"3")))
				{
					strcpy(state,"UNRCH");
				}
				else if(!(strcmp(state,"4")))
				{
					strcpy(state,"SRCQUE");
				}
				else if(!(strcmp(state,"5")))
				{
					strcpy(state,"REDIRECT");
				}
				else if(!(strcmp(state,"8")))
				{
					strcpy(state,"ECHO");
				}
				else if(!(strcmp(state,"11")))
				{
					strcpy(state,"TIMEXD");
				}
				else if(!(strcmp(state,"12")))
				{
					strcpy(state,"PARPBLM");
				}
				else if(!(strcmp(state,"13")))
				{
					strcpy(state,"TSTMREQ");
				}
				else if(!(strcmp(state,"14")))
				{
					strcpy(state,"TSTMRLY");
				}
				else if(!(strcmp(state,"15")))
				{
					strcpy(state,"REQINFO");
				}
				else if(!(strcmp(state,"16")))
				{
					strcpy(state,"INFORLY");
				}
				else if(!(strcmp(state,"17")))
				{
					strcpy(state,"ADRMREQ");
				}
				else if(!(strcmp(state,"18")))
				{
					strcpy(state,"ADRMRLY");
				}

				head=checkIfNewFlow (pcktime, prtc, saddr, daddr, sport, dport,"->", bytes, state, head);
				return;
			}
			case IPPROTO_IP:
			{
				return;
			}
			default:
//				printf("   Protocol: unknown\n");
				return;
		}
}

/* Called when reading from file */

void live_capture(char *dev)
{

	 filter_exp=NULL;

	 if (pcap_lookupnet(dev, &addr, &mask, errbuf) == -1)
	 {
			 fprintf(stderr, "Can't get netmask for device %s\n", dev);
			 addr = 0;
			 mask = 0;
	 }

	 /*  try to open device  */
	 	handle = pcap_open_live(dev, SNAP_LEN, 1, 1000, errbuf);
	 		if (handle == NULL) {
	 			fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
	 			exit(EXIT_FAILURE);
	 		}
	 /* make sure we're capturing on an Ethernet device [2] */
	 		if (pcap_datalink(handle) != DLT_EN10MB) {
	 			fprintf(stderr, "%s is not an Ethernet\n", dev);
	 			exit(EXIT_FAILURE);
	 		}
	 /* compile the filter expression */
	 		if (pcap_compile(handle, &fp, filter_exp, 0, addr) == -1) {
	 			fprintf(stderr, "Couldn't parse filter %s: %s\n",
	 			 filter_exp, pcap_geterr(handle));
	 			 exit(EXIT_FAILURE);
	 		}
	 /* apply the compiled filter */
			if (pcap_setfilter(handle, &fp) == -1) {
				fprintf(stderr, "Couldn't install filter %s: %s\n",
	 			filter_exp, pcap_geterr(handle));
	 			exit(EXIT_FAILURE);
	 		}
    /* Start capturing packets */
			pcap_loop(handle, -1, packet_received, NULL);
	/* cleanup */
			pcap_freecode(&fp);
			pcap_close(handle);
			printALL(head);
			printf("\nCapture complete.\n");


}


/* Called when reading from file */

void read_pcap_file(char *filename)
{


	filter_exp=NULL;
	handle = pcap_open_offline(filename, errbuf);
	if (handle == NULL) {
	      printf("Couldn't open pcap file %s: %s\n", filename, errbuf);
	    }

	 /* compile the filter expression */
	 	if (pcap_compile(handle, &fp, filter_exp, 0, addr) == -1) {
	 		printf("Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
	 		exit(EXIT_FAILURE);
	 		}
	 /* apply the compiled filter */
			if (pcap_setfilter(handle, &fp) == -1) {
				printf("Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handle));
	 			exit(EXIT_FAILURE);
	 		}
	/* now we can set our callback function */
			pcap_loop(handle, -1, packet_received, NULL);
	/* cleanup */

			pcap_close(handle);
			pcap_freecode(&fp);
			printALL(head);
			printf("\n");
//			printf("\nCapture complete.\n");

}


/* Main Function */

int main(int argc, char *argv[])
{
char *filename;
int valid_argument=0, i=0;
char sniffing_opt;

	/* Parsing arguments */
	for (i=1;i<argc;++i)
	{
				if(strcmp(argv[i], "-r") == 0)
					{
						if(argc>(i+1))
						{
							i++;
							filename=(char *)malloc((strlen(argv[i]) +1) * sizeof(char));
							filename= strdup(argv[i]);
							++valid_argument;
							sniffing_opt='f';
						}
						else
						{
							printf("Please enter argument value\n"); exit(0);
						}

					}

				else if(strcmp(argv[i], "-i") == 0)
				{
						if(argc>(i+1))
						{
							i++;
							dev=(char *)malloc((strlen(argv[i]) +1) * sizeof(char));
							dev= strdup(argv[i]);
							++valid_argument;
							sniffing_opt='i';
						}
						else
						{
							printf("Please enter argument value\n"); exit(0);
						}
				}

				else if(strcmp(argv[i], "-t") == 0)
				{
					if(argc>(i+1))
					{
						i++;
						if(!isdigit((unsigned char)*argv[i]))
						{ printf("Please enter valid number as a time interval.\n"); exit(0);}
						time_interval = atoi(argv[i]);
					}
					else
					{
						printf("Please enter argument value\n"); exit(0);
					}
				}

				else if(strcmp(argv[i], "-o") == 0)
				{
					if(argc>(i+1))
					{
						i++;
						if(!isdigit((unsigned char)*argv[i]))
						{ printf("Please enter valid number for time offset.\n"); exit(0);}
						time_offset = atoi(argv[i]);
					}
					else
					{
						printf("Please Enter argument value\n"); exit(0);
					}
				}

				else if(strcmp(argv[i], "-N") == 0)
				{
					if(argc>(i+1))
					{
						i++;
						if(!isdigit((unsigned char)*argv[i]))
						{
								printf("Please Enter valid number.\n");
								exit(0);
						}
						num = atoi(argv[i]);
					}
					else
					{
						printf("Please Enter argument value\n"); exit(0);
					}
				}

				else if(strcmp(argv[i], "-S") == 0)
				{
					if(argc>(i+1))
					{
						i++;
						if(!isdigit((unsigned char)*argv[i]))
						{
								printf("Please Enter valid number.\n");
								exit(0);
						}
						sec = atoi(argv[i]);
					}
					else
					{
						printf("Please Enter argument value\n"); exit(0);
					}
				}
	}

	if(valid_argument==0)
	{
		printf("Please specify arguments"
				"\n -r     Pcap file name"
				"\n -i     Network interface"
				"\n -t     Time interval in seconds"
				"\n -o     Time offset in second"
				"\n -N     First N flows"
				"\n -S     Time out for flow"
				"\n");
		return 0;
	}

/* Default 60 seconds */
	sec=60;

/* Calling specific function as per input */
	printf("\n=====================================================================================================================================");
	printf("\n Start Time\t\tProto \tSrcAddr \tSport \tDir \tDstAddr \t\tSport \tTotPKt \tTotBytes \tState \tDur");
	if(sniffing_opt=='f')
	{
		reading_from_file=1;
		read_pcap_file(filename);

	}
	else if(sniffing_opt == 'i' && time_interval >= 0){
		time_break=time(0)+time_interval;
		live_capture(dev);
	}

}
