.PHONY: mk_objs_sta mk_objs_share mk_static mk_share install sta_all share_all clean
CUR_DIR := .
INC_DIR := $(CUR_DIR)/inc
LIB_DIR := $(CUR_DIR)/lib
OBJ_DIR := $(CUR_DIR)/obj
SRC_DIR := $(CUR_DIR)/src
BIN_DIR := $(CUR_DIR)/bin
STA_DIR := $(LIB_DIR)/static
SHARE_DIR := $(LIB_DIR)/share
CC := /home/hanli/Hanli/beaglebone_black/gcc-linaro-6.5.0-2018.12-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc
INC_FLAG := -I $(INC_DIR)
LIB_NAME := snake_game
mk_objs_sta:
	$(CC) -c $(SRC_DIR)/ssd1306.c -o $(OBJ_DIR)/ssd1306.o $(INC_FLAG)
	$(CC) -c $(SRC_DIR)/button.c -o $(OBJ_DIR)/button.o $(INC_FLAG)
	$(CC) -c $(SRC_DIR)/snake.c -o $(OBJ_DIR)/snake.o $(INC_FLAG)
	$(CC) -c $(CUR_DIR)/main.c -o $(OBJ_DIR)/main.o $(INC_FLAG)
mk_objs_share:
	$(CC) -c -fPIC $(CUR_DIR)/main.c -o $(OBJ_DIR)/main.o $(INC_FLAG)
	$(CC) -c -fPIC $(SRC_DIR)/ssd1306.c -o $(OBJ_DIR)/ssd1306.o $(INC_FLAG)
	$(CC) -c -fPIC $(SRC_DIR)/snake.c -o $(OBJ_DIR)/snake.o $(INC_FLAG)
	$(CC) -c -fPIC $(SRC_DIR)/button.c -o $(OBJ_DIR)/button.o $(INC_FLAG)
mk_static:
	ar src $(STA_DIR)/lib$(LIB_NAME).a $(OBJ_DIR)/button.o $(OBJ_DIR)/ssd1306.o $(OBJ_DIR)/snake.o
mk_share:
	$(CC) -shared $(OBJ_DIR)/button.o $(OBJ_DIR)/snake.o $(OBJ_DIR)/ssd1306.o -o $(SHARE_DIR)/lib$(LIB_NAME).so
install:
	cp -f $(SHARE_DIR)/lib$(LIB_NAME).so /usr/lib
sta_all: mk_objs_sta mk_static 
	$(CC) $(OBJ_DIR)/main.o -L$(STA_DIR) -l$(LIB_NAME) -o $(BIN_DIR)/main
share_all: mk_objs_share mk_share install
	$(CC) $(OBJ_DIR)/main.o -L$(SHARE_DIR) -l$(LIB_NAME) -o $(BIN_DIR)/main
clean:
	rm -rf bin/*
	rm -rf obj/*
	rm -rf lib/static/*
	rm -rf lib/share/*

