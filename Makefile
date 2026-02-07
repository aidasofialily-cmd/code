# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall

# Source files
SRCS = src/main.cpp

# Executable name
TARGET = game

# Default target
all: $(TARGET)

# Compile and link
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

# Run the game
run: $(TARGET)
	./$(TARGET)

# Clean up build artifacts
clean:
	rm -f $(TARGET)

# Test the game
test: all
	(sleep 0.5; echo "q") | ./$(TARGET) | grep "Thanks for playing!"

# Phony targets
.PHONY: all run clean test
