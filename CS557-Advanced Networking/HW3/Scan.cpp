/*
 * Scan.cpp
 *
 *  Created on: Apr 12, 2014
 *      Author: darshan
 */

#include "Scan.h"
#include  "iostream"
#include "algorithm"

Scan::Scan() {
	// TODO Auto-generated constructor stub
}

Scan::~Scan() {
	// TODO Auto-generated destructor stub
}

void Scan::CheckIfNewdata(std::string srcIP, std::string destIP, int srcPort, int destPort, std::string protocol)
{
	int same_src_addr=0;
	int same_dst_addr=0;
	int same_port_addr=0;

	for(int i=0; i < scan_data.size(); i++)
	{
		if(scan_data.at(i).srcIp==srcIP && scan_data.at(i).protocol==protocol)			// New packet is from same source
		{
			same_src_addr=1;
			for(int j=0; j < scan_data.at(i).dest_details.size(); j++)
			{
				if(scan_data.at(i).dest_details.at(j).destIp==destIP)					// New packet is from same destination
				{
					same_dst_addr=1;
					for(int k=0; k<scan_data.at(i).dest_details.at(j).port.size(); k++)
					{
						if(scan_data.at(i).dest_details.at(j).port.at(k)==destPort)
						{
							same_port_addr=1;
							return;
						}
					}
					if(same_port_addr==0)
					{
						scan_data.at(i).dest_details.at(j).port.push_back(destPort);
						return;
					}
				}
			}

			if(same_dst_addr==0)														// If new destination address
			{
				DestDetails dest_detail;
				dest_detail.destIp=destIP;
				dest_detail.port.push_back(destPort);
				scan_data.at(i).dest_details.push_back(dest_detail);
				return;
			}
		}
	}


	if(same_src_addr==0)																// If new source address
	{
		DestDetails dest_detail;
		dest_detail.destIp=destIP;
		dest_detail.port.push_back(destPort);
		SCANDATA sd;
		sd.srcIp=srcIP;
		sd.protocol=protocol;
		sd.dest_details.push_back(dest_detail);
		scan_data.push_back(sd);
		return;
	}

	//std::cout<<"--------- "<<srcIP<<"\t"<<destIP<<"\t"<<srcPort<<"\t"<<destPort<<"\t"<<protocol<<"\n";
	return;
}

void Scan::print()
{
	for(int i=0; i<scan_data.size(); i++)												// loop for number of source addresses
	{
		std::cout<<"\n\n"<<scan_data.at(i).srcIp <<"    "<<scan_data.at(i).protocol;
		for(int j=0; j<scan_data.at(i).dest_details.size(); j++)
		{
			std::cout <<"\n\t"<<scan_data.at(i).dest_details.at(j).destIp;
			for(int k=0; k<scan_data.at(i).dest_details.at(j).port.size(); k++)
			{
				std::cout<<"\n\t\t"<<scan_data.at(i).dest_details.at(j).port.at(k);
			}
		}
	}
	return;
}


void Scan::printSummary(int h_num, int p_num)
{
	std::cout<<"\nSummary:\n";
	std::cout<<"Scanner\t\t\t#HostsScanned\t#PortsScanned\n";


	for(int i=0; i<scan_data.size(); i++)											// loop for number of source addresses
	{
		int number_of_ip_scanned=0;
		if(scan_data.at(i).dest_details.size() >= h_num)							// If scanned destinations is greater than HUNM
		{
			std::vector<int> scanned_ports;
			for(int j=0; j<scan_data.at(i).dest_details.size(); j++)
			{
				if(scan_data.at(i).dest_details.at(j).port.size() >= p_num)			// If Satisfy given PNUM value
				{
					number_of_ip_scanned++;
					for(int k=0; k<scan_data.at(i).dest_details.at(j).port.size(); k++) // Check for all ports
					{
						int added=0;
						for(int z=0; z<scanned_ports.size(); z++)
						{
							if(scan_data.at(i).dest_details.at(j).port.at(k)==scanned_ports.at(z))	// check if already in list of distinct ports
								added=1;
						}

						if(!added)
							scanned_ports.push_back(scan_data.at(i).dest_details.at(j).port.at(k));
					}
				}
			}

			if(scanned_ports.size()>=p_num)									// If scanned port greater than PNUM print value
			{
				if(!(scan_data.at(i).protocol=="ICMP"))
				{
					std::cout<<scan_data.at(i).srcIp<<"\t\t"<<number_of_ip_scanned<<"\t\t";
					std::cout<<scanned_ports.size();

					std::cout<<std::endl;
				}
			}
		}
	}
	std::cout<<std::endl;
	return;
}



void Scan::PrintVerbosity(int h_num, int p_num)
{
	std::cout<<"Scanner\t\tProto\tHostScanned\t\tPortsScanned";
	for(int i=0; i<scan_data.size(); i++)											// loop for number of source addresses
		{

//			if(scan_data.at(i).dest_details.size() >= h_num)							// If scanned destinations is greater than HUNM
			{
				for(int j=0; j<scan_data.at(i).dest_details.size(); j++)
				{
					if(scan_data.at(i).dest_details.at(j).port.size() >= p_num)			// If Satisfy given PNUM value
					{
						std::cout<<"\n"<<scan_data.at(i).srcIp<<"\t"<<scan_data.at(i).protocol<<"\t"<<scan_data.at(i).dest_details.at(j).destIp<<"\t\t";
						sort(scan_data.at(i).dest_details.at(j).port.begin(), scan_data.at(i).dest_details.at(j).port.end());
						int is_first=1;

						if(scan_data.at(i).protocol=="ICMP")				// Not to print any port for ICMP protocol
							continue;

						for(int k=0; k<scan_data.at(i).dest_details.at(j).port.size(); k++) // Check for all ports
						{
							if((k+1)<scan_data.at(i).dest_details.at(j).port.size())
							{
								if(scan_data.at(i).dest_details.at(j).port.at(k+1)==scan_data.at(i).dest_details.at(j).port.at(k)+1)
								{
									if(is_first)
										{  std::cout<<scan_data.at(i).dest_details.at(j).port.at(k)<<"-"; is_first=0;  }
								}
								else
								{
									std::cout<<scan_data.at(i).dest_details.at(j).port.at(k)<<" "; is_first=1;
								}
							}
							else
							{
								std::cout<<scan_data.at(i).dest_details.at(j).port.at(k)<<" ";
							}
						}
					}
				}
			}
		}
}


void Scan::sord_on_basic_of_srcIP(int h_num, int p_num)
{
	std::vector<SordedOnSrcIP> soip;

	for(int i=0; i<scan_data.size(); i++)
	{
		int is_there=0;
		for(int j=0; j<soip.size(); j++)
		{
			if(soip.at(j).srcIP==scan_data.at(i).srcIp)
			{	is_there=1; break; }
		}
		if(!is_there)
		{
			SordedOnSrcIP temp;
			temp.srcIP=scan_data.at(i).srcIp;
			soip.push_back(temp);
		}
	}

	for(int i=0; i<scan_data.size(); i++)
	{
		for(int j=0; j<soip.size(); j++)
		{
			if(scan_data.at(i).srcIp==soip.at(j).srcIP)
			{
				// For IP address
				for(int f=0; f<scan_data.at(i).dest_details.size(); f++)
				{
					int is_added=0;
					for(int g=0; g<soip.at(j).ip_scanned.size(); g++)
					{
						if(soip.at(j).ip_scanned.at(g)==scan_data.at(i).dest_details.at(f).destIp)
						{	is_added=1;	break;											}
					}
					if(!is_added)
					{
						soip.at(j).ip_scanned.push_back(scan_data.at(i).dest_details.at(f).destIp);
					}
				}
			}
		}
	}


	for(int i=0; i<scan_data.size(); i++)
	{
		for(int j=0; j<soip.size(); j++)
		{
			if(scan_data.at(i).srcIp==soip.at(j).srcIP)
			{
				for(int f=0; f<scan_data.at(i).dest_details.size(); f++)
				{
					if(scan_data.at(i).dest_details.at(f).port.size()>=p_num)
					{
						for(int g=0; g<scan_data.at(i).dest_details.at(f).port.size(); g++)
						{
							int is_added=0;
							for(int z=0; z<soip.at(j).port_scanned.size(); z++)
							{
								if(scan_data.at(i).dest_details.at(f).port.at(g)==soip.at(j).port_scanned.at(z))
								{ is_added=1; break; }
							}
							if(!is_added && scan_data.at(i).dest_details.at(f).port.at(g)!=0)
							{
								soip.at(j).port_scanned.push_back(scan_data.at(i).dest_details.at(f).port.at(g));
							}
						}
					}
				}
			}
		}
	}


	// Printing values

	std::cout<<"Summary:\n\n";
	std::cout<<"Scanner\t\t#HostsScanned\t#PortsScanned\n";
	for(int i=0; i<soip.size(); i++)
	{
		if(soip.at(i).ip_scanned.size()>=h_num && soip.at(i).port_scanned.size()>=p_num)
		{ std::cout<<soip.at(i).srcIP<<"\t"<<soip.at(i).ip_scanned.size()<<"\t\t"<<soip.at(i).port_scanned.size()<<std::endl; }
	}
}
