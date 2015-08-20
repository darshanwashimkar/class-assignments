This directory contains the 1.0 version of SNIFFER application
developed by Darshan as an HW1 assignment for class CS557

INFO
----

SNIFFER is an application that can capture network trafic in promiscuous
mode. It can also read captured data from a file. (pcap file format)
SNIFFER application can recognize TCP, UDP, ICMP, and IP packets. Though 
application can not recognize other protocols but it will print packet 
timestamp and packet length for other protocols.


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

*Sample as how to work with a program

1) If you want to capture packets from interface eth0 for 60 seconds
   #./sniffer.out -i eth0 -t 60

2) If you want to read pcap file(ex. DATA.PCAP) with offset 20 seconds and 
time 30 seconds (It means you want to skip the packets from DATA.PCAP file 
which are captured in first 20 sc and take a look at packets capture for 
next 30 seconds)
   #./sniffer.out -r DATA.PCAP -o 20 -t 30

3) To read complete file 
   #./sniffer.out -r DATA.PCAP

4) If you do not specify time duration while reading from interface then 
program will read till infinity.
   #./sniffer.out -i wlo1


Known issues
------------

-> Program take little while before start and end of the capture (May be 
because it is initializing and opening interfaces or it is testing network 
issue, as less packets are moving around)

-> Program will not terminate if network is not carrying any data.


Got help from
-------------

1) Tutorials present at http://www.tcpdump.org/
2) Accessed manual pages of different header files from http://www.manpagez.com/
and http://www.freebsd.org/cgi/man.cgi?manpath=FreeBSD+6.3-RELEASE
