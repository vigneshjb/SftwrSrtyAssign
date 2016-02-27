#!/usr/bin/python

from scapy.all import *
import sys
import random

#****************************** RANDOM NUMBER THING *****************************

def getRandNum():
        return random.randint(1,255)

#***************************** ICMP THING *****************************

def ICMP_echo_message(ipaddress, message, interface):
        print "sending ICMP req to", ipaddress, "with message", message, "over", interface, "\n\n"
        number = getRandNum()
        frag_counter = 0
        for item in message:
                print item, number
                ip = IP(dst=ipaddress, proto=1)
                ip.id = int(hex(ord(item)).split("x")[1] + hex(number).split("x")[1], 16)
                ip.frag = frag_counter
                pack = ip/ICMP()
                pack.show()
                sendp(pack, iface = interface)
                frag_counter = frag_counter + 1

        ip = IP(dst=ipaddress, proto=1)
        ip.id = int(hex(0).split("x")[1] + hex(number).split("x")[1], 16)
        ip.frag = frag_counter + 4096
        pack = ip/ICMP()
        pack.show()
        sendp(pack, iface = interface)
        return

#****************************** TCP THING *****************************

def TCP_message(ipaddress, message, interface):
        print "sending TCP req to ", ipaddress, " with message ", message, " over ", interface
        number = getRandNum()
        frag_counter = 0
        for item in message:
                print item, number
                ip = IP(dst=ipaddress, proto=6)
                ip.id = int(hex(ord(item)).split("x")[1] + hex(number).split("x")[1], 16)
                ip.frag = frag_counter
                pack = ip/TCP(sport=1025, dport=80, flags="S")
                pack.show()
                sendp(pack, iface = interface)
                frag_counter = frag_counter + 1

        ip = IP(dst=ipaddress, proto=1)
        ip.id = int(hex(0).split("x")[1] + hex(number).split("x")[1], 16)
        ip.frag = frag_counter + 4096
        pack = ip/TCP(sport=1025, dport=80, flags="S")
        pack.show()
        sendp(pack, iface = interface)
        return

#****************************** UDP THING *****************************

def UDP_message(ipaddress, message, interface):
        print "sending UDP req to ", ipaddress, " with message ", message, " over ", interface
        number = getRandNum()
        frag_counter = 0
        for item in message:
                print item, number
                ip = IP(dst=ipaddress, proto=17)
                ip.id = int(hex(ord(item)).split("x")[1] + hex(number).split("x")[1], 16)
                ip.frag = frag_counter
                pack = ip/UDP(dport=53)
                pack.show()
                sendp(pack, iface = interface)
                frag_counter = frag_counter + 1

        ip = IP(dst=ipaddress, proto=1)
        ip.id = int(hex(0).split("x")[1] + hex(number).split("x")[1], 16)
        ip.frag = frag_counter + 4096
        pack = ip/UDP(dport=53)
        pack.show()
        sendp(pack, iface = interface)
        return

# ******************************** MAIN **********************************

if len(sys.argv) == 1:
	ICMP_echo_message("0.0.0.0", " 123", "eth0")
	TCP_message("0.0.0.0", " 123", "eth0")
	UDP_message("0.0.0.0", " 123", "eth0")

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

