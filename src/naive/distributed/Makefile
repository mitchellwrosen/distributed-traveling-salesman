CC=mpic++
CFLAGS=-Wall -std=c++11
LDFLAGS=-Wall

SRCS=$(wildcard *.cpp **/*.cpp **/**/*.cpp)
OBJS=$(patsubst %.cpp,%.o,$(SRCS))
INC=-I.
LIBS=-lmpi

all: $(OBJS)
	$(CC) $(LDFLAGS) $(INC) $(OBJS) $(LIBS) -o naive-distributed

%.o: %.cpp
	$(CC) $(INC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS)
