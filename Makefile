# Makefile

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Iinclude

# Sources and objects
SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst src/%.cpp,src/%.o,$(SRC))

# Output binary
TARGET = psi 

# Default rule
all: $(TARGET)

# Linking
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET)

# Compile .cpp to .o
src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f src/*.o $(TARGET)
