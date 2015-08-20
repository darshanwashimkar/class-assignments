This directory contains the 1.0 version of FLOW SNIFFER application
developed by Darshan as an HW2 assignment for class CS557

INFO
----

FLOW SNIFFER is an application that can be used to capture and analyze network 
trafic flows. FLOW SNIFFER also support data reading from file (pcap file format)
FLOW SNIFFER application can recognize TCP, UDP and ICMP packet flows. 


Compailing instructions
-----------------------

Run below command to create executable file (sniffer.out)
# make all
*make sure you have libpcap installed in your system.

How to use program
-------------------
*Program takes following arguments 

(-r) ------- Specify pcap file name to read already captured data
(-i) ------- Specify interface to capture live data
(-t) ------- Time duration for which you want to capture data (in seconds)
(-o) ------- Time offset (used when reading Pcap file ) (in seconds)
(-N) ------- For number of flows to be printed (whole number)
(-S) ------- Time out interval for the flow


*Sample as how to work with a program

1) If you want to analyze flows from file hart_ip.pcap
   # ./fsniffer.out -r hart_ip.pcap 

2) If you want to read pcap file(ex. DATA.PCAP) with offset 20 seconds and 
time 30 seconds (It means you want to skip the packets flows from DATA.PCAP file 
which are captured in first 20 sc and take a look at packets flow of 
next 30 seconds)
   #./fsniffer.out -r DATA.PCAP -o 20 -t 30

3) To take look at first 5 packets flows from file DATA.PCAP
   #./fsniffer.out -r DATA.PCAP -N 5

4) If you do not specify time duration while reading from interface then 
program will read till infinity.
   #./fsniffer.out -i wlo1


Known issues
------------

-> Application don't show directions of tcp flows packets correctly

-> Program will not terminate if network is not carrying any data.

-> Program shows one warning on compilation.


Got help from
-------------
1) http://qosient.com/argus/ for testing purpose
2) Accessed manual pages of different header files from http://www.manpagez.com/
and http://www.freebsd.org/cgi/man.cgi?manpath=FreeBSD+6.3-RELEASE
