CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11
TARGET = cpu
SRCS = main.cpp CPU.cpp
OBJS = $(SRCS:.cpp=.o)
DEPS = CPU.h

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
