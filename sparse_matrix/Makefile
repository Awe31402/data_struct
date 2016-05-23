SRC = sparse.c main.c
HEADERS = base.h sparse.h

OPTIONS = -Wall -fms-extensions

all: $(SRC) $(HEADERS)
	gcc -pg *.c -o main  $(OPTIONS)
gprof:
	./main
	gprof -b ./main
clean:
	rm main *.out
