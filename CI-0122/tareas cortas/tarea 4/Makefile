# PThreads makefile example
#
# Author: CI0123 Proyecto integrador de redes y sistemas operativos
# Date: 2020/Ago/31
# Repaso: material de concurrencia
#

HelloPT: HelloPT.c
	gcc -g HelloPT.c -o HelloPT -lpthread

Condition.o: Condition.cc Condition.h
	g++ -g -c Condition.cc

Lock.o: Lock.cc Lock.h
	g++ -g -c Lock.cc

Mutex.o: Mutex.cc Mutex.h
	g++ -g -c Mutex.cc

Semaforo.o: Semaforo.cc Semaforo.h
	g++ -g -c Semaforo.cc

PiPorSeries.o: PiPorSeries.cc Mutex.h
	g++ -g -c PiPorSeries.cc

PiPorSeries: PiPorSeries.o Mutex.o
	g++ -g PiPorSeries.o Mutex.o -o PiPorSeries -lpthread

sumaUno: sumaUno.c
	gcc -g sumaUno.c -o sumaUno

sumaUnoPT.o: sumaUnoPT.cc Mutex.h
	g++ -g -c sumaUnoPT.cc

sumaUnoPT: sumaUnoPT.o Mutex.o
	g++ -g sumaUnoPT.o Mutex.o -o sumaUnoPT -lpthread

ThreadBase.o: ThreadBase.cc ThreadBase.h
	g++ -g -c ThreadBase.cc

sumaUnoThread.o: sumaUnoThread.cc Thread.h Mutex.h
	g++ -g -c sumaUnoThread.cc

sumaUnoThread: sumaUnoThread.o ThreadBase.o Thread.o Mutex.o 
	g++ -g sumaUnoThread.o ThreadBase.o Thread.o Mutex.o -lpthread -o sumaUnoThread
