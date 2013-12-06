CC=mpic++
CFLAGS=-Wall
LDFLAGS=-Wall

SRCS=$(wildcard *.cpp **/*.cpp **/**/*.cpp)
OBJS=$(patsubst %.cpp,%.o,$(SRCS))
INC=-I.
LIBS=-lmpi
EXE=naive-single

all: $(EXE)

debug: CC += -g -DDEBUG
debug: $(EXE)

optimized: CC += -O3
optimized: $(EXE)

$(EXE): $(OBJS)
	$(CC) $(LDFLAGS) $(INC) $(OBJS) $(LIBS) -o $(EXE)

%.o: %.cpp
	$(CC) $(INC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS) $(EXE)
