#!/usr/bin/python

from scapy.layers.inet import IP, ICMP, UDP
from scapy.sendrecv import sr
import sys
import random

def ICMP_echo_message(ipaddress, message, interface):
	print "sending ICMP req to ", ipaddress, " with message ", message, " over ", interface
	return

def TCP_message(ipaddress, message, interface):
	print "sending TCP req to ", ipaddress, " with message ", message, " over ", interface
	return

def UDP_message(ipaddress, message, interface):
	print "sending UDP req to ", ipaddress, " with message ", message, " over ", interface
	return

def getRandNum():
	return random.randint(1,255)

if len(sys.argv)!= 5:
	sys.exit("Incorrect program execution")

print 'Number of arguments:', len(sys.argv)
ip_address = sys.argv[1]
interface = sys.argv[2]
type_param = sys.argv[3]
message = sys.argv[4]

if type_param == "0":
	ICMP_echo_message(ip_address, message, interface)
elif type_param == "1":
	TCP_message(ip_address, message, interface)
elif type_param == "2":
	UDP_message(ip_address, message, interface)

print getRandNum()
