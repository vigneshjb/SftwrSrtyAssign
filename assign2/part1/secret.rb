
abort("Incorrect program execution") unless ARGV.length>3

ip_address = ARGV[0]
interface = ARGV[1]
type = ARGV[2]
message = ARGV[3]

puts "ip address = #{ip_address} \ninterface = #{interface} \ntype = #{type} \nmessage = #{message}"
