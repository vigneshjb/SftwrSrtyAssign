require 'socket'
require 'rubygems'
require 'racket'

# **************************** SUB METHODS ****************************
def ICMPRequest(ipaddress, interface, message)
        p "Sending ICMPpacket to #{ipaddress} on #{interface} with message #{message}"
end

def TCPSYN(ipaddress, interface, message)
        p "Sending TCPSYNC packet to #{ipaddress} on #{interface} with message #{message}"
end

def UDP(ipaddress, interface, message)
        p "Sending UDP packet to #{ipaddress} on #{interface} with message #{message}"
	n = Racket::Racket.new
  	n.iface = "eth0"
	n.l3 = IPv4.new
	n.l3.src_ip = Socket::getaddrinfo(Socket.gethostname,"echo",Socket::AF_INET)[0][3]
	n.l3.dst_ip = ipaddress
	n.l3.protocol = 0x11
	n.l4 = UDP.new
	n.l4.src_port = 1024 + rand(65535-1024)
	n.l4.dst_port = 80
	n.l4.payload = message
	n.l4.fix!(n.l3.src_ip, n.l3.dst_ip)
#	f = n.sendpacket
	n.layers.compact.each do |l|
		puts l.pretty
	end

end

# *************************** MAIN STUFF *************************

abort("Incorrect program execution") unless ARGV.length>3

ip_address = ARGV[0]
interface = ARGV[1]
type = ARGV[2]
message = ARGV[3]

puts "ip address = #{ip_address} \ninterface = #{interface} \ntype = #{type} \nmessage = #{message}"

if (type == "0")
	ICMPRequest(ip_address, interface, message)
elsif (type == "1")
	TCPSYN(ip_address, interface, message)
elsif (type == "2")
	UDP(ip_address, interface, message)
end

