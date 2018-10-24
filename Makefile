#Makefile

BUILD_DIR ?= ./bin
SRC_DIRS ?= ./src
INC_DIRS ?= ./inc

TARGET ?= chord
SRCS := chord.cpp
OBJ = $(ccsrc:.cpp=.o)



$(TARGET): $(OBJ)
	g++ -I $(INC_DIRS) $(LDFLAGS) $(CPPFLAGS) -lssl -lcrypto -o $(BUILD_DIR)/chord $(SRC_DIRS)/chord.cpp $(SRC_DIRS)/node.cpp $(SRC_DIRS)/fingertable.cpp

clean:
	$(RM) $(BUILD_DIR)/chord