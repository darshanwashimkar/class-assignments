#include <stdio.h>
#include <pcap.h>
#include <time.h>
#include <stdlib.h>
#include "string.h"
#include <errno.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/ethernet.h>
#include <sniffer.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <cctype>
#include <time.h>
#include "Scan.h"

#define HNUM  1
#define PNUM  1

char *filename;                  /* Pcap file Name*/
int h_num, p_num;				 /* hosts and the number of ports that a host must scan to be considered scanning */
int sec;
int ver;
Scan S;


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
	const u_char *payload;                    /* Packet payload */
	int size_ip;
	int size_tcp;
	int size_payload;

	/* While reading from file */

	if(no_of_call==1 && reading_from_file==1)
	{
		no_of_call++;
		time_break=(const time_t)(header->ts.tv_sec)+(const time_t)time_interval+(const time_t)time_offset;
		time_start=(const time_t)(header->ts.tv_sec)+(const time_t)time_offset;
		//printf("%s", ctime(&time_break));
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

	/* Print packet count */
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

	/* determine protocol */
		switch(ip->ip_p) {
			case IPPROTO_TCP:
			{
				/* define/compute TCP header offset */
				tcp = (struct sniff_tcp*)(packet + SIZE_ETHERNET + size_ip);
				size_tcp = TH_OFF(tcp)*4;
				if (size_tcp < 20) {
					/* Invalid TCP header length */
					return;
				}

				/* define/compute tcp payload (segment) offset */
				payload = (u_char *)(packet + SIZE_ETHERNET + size_ip + size_tcp);

				/* compute tcp payload (segment) size */
				size_payload = ntohs(ip->ip_len) - (size_ip + size_tcp);
				std::string srcIP(inet_ntoa(ip->ip_src)), destIP(inet_ntoa(ip->ip_dst));
				int srcPort  = ntohs(tcp->th_sport);
				int destPort = ntohs(tcp->th_dport);

				if(!(tcp->th_flags & TH_RST))
				{
					S.CheckIfNewdata(srcIP ,destIP, srcPort, destPort, "TCP");
				}

				return;
			}
			case IPPROTO_UDP:
			{
				/* Print protocol type  */
//				printf("   Protocol: UDP\n");
				udp = (struct udphdr*)(packet + SIZE_ETHERNET + size_ip);

				std::string srcIP(inet_ntoa(ip->ip_src)), destIP(inet_ntoa(ip->ip_dst));
				int srcPort  = ntohs(udp->uh_sport);
				int destPort = ntohs(udp->uh_dport);

				S.CheckIfNewdata(srcIP ,destIP, srcPort, destPort, "UDP");

				return;
			}

			case IPPROTO_ICMP:
			{
				/* Print protocol type  */

//				printf("   Protocol: ICMP\n");
				icmp = (struct icmphdr*)(packet + SIZE_ETHERNET + size_ip);

				std::string srcIP(inet_ntoa(ip->ip_src)), destIP(inet_ntoa(ip->ip_dst));

				S.CheckIfNewdata(srcIP ,destIP, 0, 0, "ICMP");

				return;
			}

			default:
				return;
		}
}




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

			//printf("\nCapture complete.\n");


}

void read_pcap_file(char *filename)
{


	filter_exp=NULL;
	handle = pcap_open_offline(filename, errbuf);
	if (handle == NULL) {
	      printf("Couldn't open pcap file %s: %s\n", filename, errbuf);
	      exit(0);
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
		//	printf("\nCapture complete.\n");

}


int main(int argc, char *argv[])
{
	int i;
	int valid_argument=0;
	char sniffing_opt='u';
	time_interval=0;
	time_offset=0;
	reading_from_file=0;
	ver=0;

	/* Initialize the values for number of host and number of ports */
	h_num=HNUM;
	p_num=PNUM;

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

				else if(strcmp(argv[i], "-h") == 0)
				{
					if(argc>(i+1))
					{
						i++;
						if(!isdigit((unsigned char)*argv[i]))
						{
								printf("Please Enter valid number of hosts.\n");
								exit(0);
						}
						h_num = atoi(argv[i]);
					}
					else
					{
						printf("Please Enter argument value\n"); exit(0);
					}
				}

				else if(strcmp(argv[i], "-p") == 0)
				{
					if(argc>(i+1))
					{
						i++;
						if(!isdigit((unsigned char)*argv[i]))
						{
								printf("Please Enter valid number of ports.\n");
								exit(0);
						}
						p_num = atoi(argv[i]);
					}
					else
					{
						printf("Please Enter argument value\n"); exit(0);
					}
				}
				else if(strcmp(argv[i], "-V") == 0)
				{
					ver=1;
				}
				else if(strcmp(argv[i], "-S") == 0)
				{
					if(argc>(i+1))
					{
						i++;
						if(!isdigit((unsigned char)*argv[i]))
						{
								printf("Please Enter valid number of seconds.\n");
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
				"\n -S     Specifies the timeout interval for a flow"
				"\n -h     The number of hosts that a host must scan to be considered scanning"
				"\n -p     The number of ports that a host must scan to be considered scanning"
				"\n");
		return 0;
	}

	/* Calling specific function as per input */

	if(sniffing_opt=='f')
	{
		reading_from_file=1;
		read_pcap_file(filename);

	}
	else if(sniffing_opt == 'i' && time_interval >= 0){
		time_break=time(0)+time_interval;
		live_capture(dev);
	}
	else
	{
		printf("Invalid arguments"
				"\n -r     Pcap file name"
				"\n -i     Network interface"
				"\n -t     Time interval in seconds"
				"\n -o     Time offset in second"
				"\n -S     Specifies the timeout interval for a flow"
				"\n -h     The number of hosts that a host must scan to be considered scanning"
				"\n -p     The number of ports that a host must scan to be considered scanning"
				"\n");
	}

//	S.print();

	if(ver)
	{	S.PrintVerbosity(h_num, p_num); }
	printf("\n");
	S.sord_on_basic_of_srcIP(h_num, p_num);
	printf("\n");
	return 1;
}
