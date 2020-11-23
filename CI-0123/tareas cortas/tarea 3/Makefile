all: udpS udpC udpC_Brdc

udpS: udp-server.cc Socket.cc
	g++ udp-server.cc Socket.cc -o udpS

udpC: udp-client.cc Socket.cc
	g++ udp-client.cc Socket.cc -o udpC

udpC_Brdc: udp-client-brdc.cc Socket.cc
	g++ udp-client-brdc.cc Socket.cc -o udpC_Brdc

# cli-tcp.out: cli-tcp.c
# 	gcc -g cli-tcp.c -o cli-tcp.out

# serv-tcp.out: serv-tcp.c
# 	gcc -g serv-tcp.c -o serv-tcp.out

# int-tcp.out: int-tcp.c
# 	gcc -g int-tcp.c -o int-tcp.out