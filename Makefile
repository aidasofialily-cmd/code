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

# Run automated tests
test: $(TARGET)
	@echo "Running tests..."
	@echo "q" | ./$(TARGET) | grep -q "Thanks for playing!"
	@echo "Tests passed!"

# Phony targets
.PHONY: all run clean test
