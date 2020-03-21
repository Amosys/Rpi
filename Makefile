OBJ 		:= Server

#HOST		:= arm-linux-gnueabihf-
HOST		:= 
CC 			:= $(HOST)gcc
CPP 		:= $(HOST)g++
CPPFLAG 	:= -std=c++11 -Wall -g
LIB			:= -lpthread

CURRENT_DIR := $(shell pwd)
INC_DIR		:= $(CURRENT_DIR)/inc
SRC_DIR		:= $(CURRENT_DIR)/src

CPP_FILE	:= $(shell find $(SRC_DIR) -name "*.cpp")

ALL:
	$(CPP) $(CPPGLAGS) -I$(INC_DIR) $(CPP_FILE) main.cpp $(LIB) -o $(OBJ)

.PHONY:clean
clean:
	rm $(OBJ) -rf