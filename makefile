CC = g++
CFLAGS = -g -fno-common
source_files = $(wildcard *.cpp)
header_files = $(wildcard *.h)
object_files = $(source_files:.cpp=.o)

exciplex: $(source_files)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

game.o: game.cpp $(header_files)
	$(CC) $(CFLAGS) -c -o game.o game.cpp
