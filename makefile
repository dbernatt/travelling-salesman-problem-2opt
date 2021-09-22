
CCFLAGS = -Wall
CC = g++

TSP : TSP.o
	$(CC) TSP.o -o TSP  -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network -lGL

TSP.o : TSP.cpp
	$(CC) -c TSP.cpp
.PHONY : clean
clean:
	rm -f TSP TSP.o core *~