#!/usr/bin/python

from scapy.all import *
import sys
import random

def getRandNum():
        return random.randint(1,255)

def ICMP_echo_message(ipaddress, message, interface):
	print "sending ICMP req to ", ipaddress, " with message ", message, " over ", interface
	number = getRandNum()
	for item in message:
		ip = IP(src="127.0.0.1", dst=ipaddress, proto=1)
		ip.id = int(hex(ord(item)).split("x")[1] + hex(number).split("x")[1], 16)
		pack = ip/ICMP()
		pack.show()
		sendp(pack, iface = interface)
	return

def TCP_message(ipaddress, message, interface):
	print "sending TCP req to ", ipaddress, " with message ", message, " over ", interface
	number = getRandNum()
        for item in message:
                ip = IP(src="127.0.0.1", dst=ipaddress, proto=6)
                ip.id = int(hex(ord(item)).split("x")[1] + hex(number).split("x")[1], 16)
                pack = ip/TCP(sport=1025, dport=80, flags="S")
		pack.show()
                sendp(pack, iface = interface)
        return

def UDP_message(ipaddress, message, interface):
	print "sending UDP req to ", ipaddress, " with message ", message, " over ", interface
	number = getRandNum()
        for item in message:
                ip = IP(src="127.0.0.1", dst=ipaddress, proto=17)
                ip.id = int(hex(ord(item)).split("x")[1] + hex(number).split("x")[1], 16)
                pack = ip/UDP(dport=53)
		pack.show()
                sendp(pack, iface = interface)
	return

if len(sys.argv)!= 5:
	sys.exit("Incorrect program execution : please use ./secret_sender <ip_address> <interface> <type> <message>")

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

