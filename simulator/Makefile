# Makefile for LiPo Battery Tester Simulator

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
TARGET = lipo_simulator
SRC = main.cpp

# Default target
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) -lpthread

# Clean build artifacts
clean:
	rm -f $(TARGET)

# Run demo mode
demo: $(TARGET)
	./$(TARGET) demo

# Run interactive mode
interactive: $(TARGET)
	./$(TARGET) interactive

# Run monitor mode
monitor: $(TARGET)
	./$(TARGET) monitor

.PHONY: all clean demo interactive monitor
