all: clean program run

program: fileManager.c
	gcc -o fileManager fileManager.c

run:
	./fileManager

clean:
	rm -f fileManager