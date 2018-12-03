#Makefile

BUILD_DIR ?= ./bin
SRC_DIRS ?= ./src
INC_DIRS ?= ./include

TARGET ?= chord
SRCS := chord.cpp
OBJ = $(ccsrc:.cpp=.o)

_DEBUG = 0

$(TARGET): $(OBJ)
	g++ -I $(INC_DIRS) $(LDFLAGS) $(CPPFLAGS)  -o $(BUILD_DIR)/chord $(SRC_DIRS)/chord.cpp $(SRC_DIRS)/node.cpp $(SRC_DIRS)/fingertable.cpp

clean:
	$(RM) $(BUILD_DIR)/chord