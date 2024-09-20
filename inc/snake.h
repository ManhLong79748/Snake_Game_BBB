#pragma once

#ifndef SNAKE_H
#define SNAKE_H

#include "button.h"
#include "ssd1306.h"

#define SNAKE_ARRAY_SIZE 310

int Snake_getGameSpeed();
int Snake_collisionSnake(int x, int y, int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength, int detect) ;
void Snake_generateFood(int fd, int foodXY[], int width, int height, int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength) ;
void Snake_prepairSnakeArray(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength) ;
void Snake_moveSnakeArray(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength, int direction) ;
void Snake_move(int fd, int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength, int direction) ;
int Snake_eatFood(int snakeXY[][SNAKE_ARRAY_SIZE], int foodXY[]);
int Snake_collisionDetection(int snakeXY[][SNAKE_ARRAY_SIZE], int ScreenWidth, int ScreenHeight, int snakeLength);
void Snake_loadSnake(int fd, int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength);
void Snake_refreshInfoBar(int fd, int score, int speed) ;
void Snake_gameOverScreen(int fds, int fdb, char *buff, size_t size) ;
void Snake_gameWin(int fds, int fdb, char *buff, size_t size) ;
void Snake_startGame(int fds, int fdb, char *buff, size_t size, int snakeXY[][SNAKE_ARRAY_SIZE], int foodXY[], int ScreenWidth, int ScreenHeight, int snakeLength, int direction, int score, int speed) ;
void Snake_loadGame(int fds, int fdb, char *buff, size_t size) ;

#endif