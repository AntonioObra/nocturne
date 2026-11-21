CXX = clang++
CXXFLAGS = -std=c++20 -Wall -Wextra -g -Iinclude

SRC = main.cpp $(wildcard src/*.cpp)
TARGET = dungeon

.PHONY: all run clean

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)
