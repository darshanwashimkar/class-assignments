This directory contains the 1.0 version of FDSCAN (Network Scanner detector 
tool) developed by Darshan as an HW3 assignment for class CS557

INFO
----

FDSCAN is an application that can detect any scanning activity in network.
FDSCAN can be used in two ways, either it can capture live packets or user 
can give pcap file name for already captured packet data. FDSCAN application 
can recognize TCP, UDP, ICMP.


Compailing instructions
-----------------------

Run below command to create executable file (sniffer.out)
# make all
*make sure you have libpcap installed in your system.

To remove all tool output files
#make clean

To run
#./fdscan [options...]


How to use program
-------------------
*Program takes following arguments 

(-r) ------- Specify pcap file name to read already captured data
(-i) ------- Specify interface to capture live data
(-t) ------- Time duration for which you want to capture data (in seconds)
	     (NOTE: This option is must when you are capturing live data)
(-o) ------- Time offset (used when reading Pcap file ) (in seconds)
(-S) ------- Specifies the timeout interval for a flow (S is 60s in default).
(-h) ------- The number of hosts that a host must scan to be considered scanning.
(-p) ------- The number of ports that a host must scan to be considered scanning.
(-V) ------- The verbosity option

Note: Default value of -p and -h is 1 because scanner may perform either
horizontal scan or vertical scan, So to detect both this type of scanning, default
values of number of ports and number of host is 1. User then can narrow down 
depending of default result.

*Sample as how to work with a program

1) For horizontal scanner detection, you can keep value of -p as 1
   #./fdscan -r example7.pcap -h 10 -p 1 -V

2) For vertical scanner detection, you can keep value of -h as 1
   #./fdscan -r example7.pcap -h 1 -p 20 -V

Known issues
------------

-> Any activity similar to Scanner is also logged in the FDSACN.


Got help from
-------------

1) http://nmap.org/

