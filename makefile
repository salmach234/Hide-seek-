CXX = g++
CXXFLAGS = -Wall -std=c++17 -Iinclude -I/opt/homebrew/include

LDFLAGS = -L/opt/homebrew/lib \
-lsfml-graphics -lsfml-window -lsfml-system

SRC = $(wildcard src/*.cpp)
TARGET = game

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

run: all
	./game