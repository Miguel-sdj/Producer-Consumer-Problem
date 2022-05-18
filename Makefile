all: problem

problem: main.c
	gcc -pthread -o problem main.c

.PHONY: clean run

run:
	./problem

clean:
	rm -rf problem