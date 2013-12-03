CC=mpic++
CFLAGS=-Wall -mpopcnt -std=c++11
LDFLAGS=-Wall

SRCS=$(wildcard *.cpp **/*.cpp **/**/*.cpp)
OBJS=$(patsubst %.cpp,%.o,$(SRCS))
INC=-I.
LIBS=-lmpi

all: held-karp-distributed

debug: CC += -g -DDEBUG
debug: held-karp-distributed

held-karp-distributed: $(OBJS)
	$(CC) $(LDFLAGS) $(INC) $(OBJS) $(LIBS) -o held-karp-distributed

%.o: %.cpp
	$(CC) $(INC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS)
