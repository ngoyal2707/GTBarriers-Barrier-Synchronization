OMPFLAGS = -fopenmp
OMPLIBS = -lgomp

CC = gcc
CPPFLAGS = -g -Wall $(OMPFLAGS)
LDFLAGS = -g -Wall
LDLIBS = $(OMPLIBS)

GTMP_SRC = gtmp.c
GTMP_OBJ = $(patsubst %.c,%.o,$(GTMP_SRC))

all: hello_world

hello_world: hello_world.o $(GTMP_OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS) 

clean:
	rm -f *.o hello_world
