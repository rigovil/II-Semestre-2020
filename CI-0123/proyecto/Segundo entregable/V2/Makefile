all: covid_data client

covid_data: main.cc ./sources/Parser.cc ./sources/Reader.cc ./sources/Socket.cc
	g++ -w main.cc ./sources/Parser.cc ./sources/Reader.cc ./sources/Socket.cc -o covid_data -lpthread -lssl -lcrypto

client: MirrorClientTest.cc ./sources/Socket.cc
	g++ -w MirrorClientTest.cc ./sources/Socket.cc -o client -lssl -lcrypto