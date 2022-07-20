LFLAGS = -pthread -lboost_program_options -lboost_thread-mt -lboost_serialization -lboost_system
LIBS += -lboost_serialization
LIBS += -lboost_system

main : mergesort.o minheap.o main.o
	clang++ mergesort.o minheap.o main.o -o main

main.o : main.cpp mergesort.o
	clang++ -c main.cpp -std=c++17

minheap.o : minheap.cpp minheap.h
	clang++ -c minheap.cpp -std=c++17

mergesort.o : mergesort.cpp mergesort.h
	clang++ -c mergesort.cpp -std=c++17

clean :
	-rm *.o $(objects) main
