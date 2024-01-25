all: clean compile run

compile: main.cpp board.cpp board.h pieces.cpp pieces.h
	@echo "-------------------------------------------"
	@echo "Compiling..."
	@g++ -o test main.cpp board.cpp pieces.cpp

run:
	@echo "-------------------------------------------"
	@echo "Running the tests...."
	@echo "======================================================================="
	@./test
	@echo "======================================================================="
	@echo "Completed tests...."

clean:
	@echo "-------------------------------------------"
	@echo "Cleaning..."
	@rm -f *.o
	@rm -f test
