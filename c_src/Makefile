CC=gcc
SOURCES=main.c interaction-combinators.c
FLAGS=-o main
TEST_SOURCES=test.c interaction-combinators.c
TEST_FLAGS=-o test
INC=-I./headers/

all:
	$(CC) $(SOURCES) $(INC) $(FLAGS)

test:
	$(CC) $(TEST_SOURCES) $(INC) $(TEST_FLAGS)

clean:
	rm main
	rm test
