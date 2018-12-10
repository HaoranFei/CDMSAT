CC = g++
CFLAGS = -std=c++11 
EXEC = solver.out
OLEVEL = -O3

compile: 
	$(CC) $(CFLAGS) $(OLEVEL) -o $(EXEC) solver.cpp 

clean:
	rm -f $(EXEC)