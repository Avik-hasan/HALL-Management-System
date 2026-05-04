CXX = g++
CXXFLAGS = -std=c++17 -Wall

TARGET = hall_system.exe
SRC = main.cpp

# Default target
all: $(TARGET)

# Compile
$(TARGET): $(SRC) models.h manager.h color.h
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# Clean up built files
clean:
	del $(TARGET)

# Run the program
run: $(TARGET)
	.\$(TARGET)
