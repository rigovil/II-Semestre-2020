all: server client

server: server.cc ./sources/Parser.cc ./sources/Reader.cc ./sources/Socket.cc
	g++ server.cc ./sources/Parser.cc ./sources/Reader.cc ./sources/Socket.cc -o server -lpthread -lssl -lcrypto

client: client.cc ./sources/Socket.cc
	g++ client.cc ./sources/Socket.cc -o client -lssl -lcrypto