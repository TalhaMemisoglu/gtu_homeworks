all: clean compile run

compile: main.cpp directory.h directory.cpp file.h rfile.h rfile.cpp slfile.h slfile.cpp OSManager.h OSManager.cpp
	@echo "-------------------------------------------"
	@echo "Compiling..."
	@g++ -o test main.cpp directory.cpp OSManager.cpp rfile.cpp slfile.cpp

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
