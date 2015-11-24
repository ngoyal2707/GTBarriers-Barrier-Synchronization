OMPFLAGS = -fopenmp
OMPLIBS = -lgomp -lm

CC = gcc
CPPFLAGS = -g -Wall $(OMPFLAGS)
LDFLAGS = -g -Wall
LDLIBS = $(OMPLIBS) 

#GTMP_SRC = gtmp.c
#GTMP_OBJ = $(patsubst %.c,%.o,$(GTMP_SRC))

all: dissemination_barrier

hello_world: dissemination_barrier.o 
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS) 

clean:
	rm -f *.o dissemination_barrier
