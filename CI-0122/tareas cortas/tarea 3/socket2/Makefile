all: MirrorClient MirrorServer 

MirrorClient: MirrorClientTest.cc Socket.cc
	g++ MirrorClientTest.cc Socket.cc -o MirrorClient

MirrorServer: MirrorServerTest.cc Socket.cc
	g++ MirrorServerTest.cc Socket.cc ThreadBase.cc -o MirrorServer -lpthread

