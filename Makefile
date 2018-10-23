#Makefile

BUILD_DIR ?= ./bin
SRC_DIRS ?= ./src
INC_DIRS ?= ./inc

TARGET ?= chord
SRCS := chord.cpp


$(TARGET):
	g++ -I $(INC_DIRS) -o $(BUILD_DIR)chord $(SRC_DIRS)/chord.cpp
	#mv chord $(BUILD_DIR)/chord

clean:
	$(RM) -f $(BUILD_DIR)/chord