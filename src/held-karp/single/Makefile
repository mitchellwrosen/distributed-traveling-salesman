CC=mpic++
CFLAGS=-Wall -mpopcnt -std=c++11
LDFLAGS=-Wall

SRCS=$(wildcard *.cpp **/*.cpp **/**/*.cpp)
OBJS=$(patsubst %.cpp,%.o,$(SRCS))
INC=-I.
LIBS=-lmpi

all: held-karp-single

debug: CC += -g -DDEBUG
debug: held-karp-single

held-karp-single: $(OBJS)
	$(CC) $(LDFLAGS) $(INC) $(OBJS) $(LIBS) -o held-karp-single

%.o: %.cpp
	$(CC) $(INC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS)
