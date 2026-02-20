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

# Run a simple sanity test
test: $(TARGET)
	@echo "Running sanity test..."
	@(sleep 0.5; echo ' '; sleep 4; echo 'q') | ./$(TARGET) | grep "Thanks for playing!"
	@echo "Test passed!"

# Phony targets
.PHONY: all run clean test
