all: dataServer interServer client

dataServer: dataServer.cc ./sources/Parser.cc ./sources/Reader.cc ./sources/Socket.cc ./sources/Response.cc
	g++ dataServer.cc ./sources/Parser.cc ./sources/Reader.cc ./sources/Socket.cc ./sources/Response.cc -o dataServer -lpthread -lssl -lcrypto

interServer: interServer.cc ./sources/Connection.cc ./sources/Socket.cc
	g++ interServer.cc ./sources/Connection.cc ./sources/Socket.cc -w -o interServer -lpthread -lssl -lcrypto

client: client.cc ./sources/Socket.cc
	g++ client.cc ./sources/Socket.cc -o client -lssl -lcrypto