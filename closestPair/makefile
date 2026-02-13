CC=g++
CFLAGS=-g -O3 -std=c++11 -Wall
OBJS=driver.o point.o closestpair.o

# support for overriding the cutoff value
CUTOFF ?= 3         # default value
CFLAGS += -DCUTOFF=$(CUTOFF)

driver: $(OBJS)
	$(CC) $(CFLAGS) -o driver $(OBJS)

point.o: point.h
closestpair.o: closestpair.h point.h

# the following rule is used to compile .cpp files to .o
.cpp.o:
	$(CC) $(CFLAGS) -c $<

# the following removes all .o files, but leaves the executable
clean:
	rm -f *.o
