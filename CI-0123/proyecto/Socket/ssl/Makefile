

# Makefile Semana 4
#

SSLtest: SSLtest.o Socket.o
	g++ -g SSLtest.o Socket.o -o SSL -lssl -lcrypto

Socket.o: Socket.h Socket.cc
	g++ -g -c Socket.cc -lssl -lcrypto

SSLtest.o: SSLtest.cc Socket.h
	g++ -g -c SSLtest.cc -lssl -lcrypto
