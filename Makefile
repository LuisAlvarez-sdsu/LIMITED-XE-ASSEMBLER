# Assignment #2 LXE Assembler
# CS530-05-Spr2026
# Team Memebers: Luis Alvarez, Jazmin Gallegos
# MAKEFILE

CXX = g++
CXXFLAGS = -Wall -std=c++17 -g

TARGET = lxe

SRCS = lxe.cpp
OBJS = $(SRCS:.cpp=.o)

# Default build
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Compile rule
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build + outputs

clean:
	rm -f $(OBJS) $(TARGET) *.l *.st

# Rebuild from scratch
rebuild: clean all