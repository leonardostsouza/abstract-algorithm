CC=gcc
SOURCES=main.c interaction-combinators.c
FLAGS=-o main -O2
DBG_FLAGS = -Wall -D__DEBUG__ -o main_dbg
TEST_SOURCES=test.c interaction-combinators.c
TEST_FLAGS=-o test
INC=-I./headers/

all:
	$(CC) $(SOURCES) $(INC) $(FLAGS)

debug:
	$(CC) $(SOURCES) $(INC) $(DBG_FLAGS)

test:
	$(CC) $(TEST_SOURCES) $(INC) $(TEST_FLAGS)

clean:
	rm main
	rm main_dbg
	rm test
