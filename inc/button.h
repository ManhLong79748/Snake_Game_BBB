#pragma once

#ifndef BUTTON_H
#define BUTTON_H

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

//Linux Libraries
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#define SSD1306_BUTTON_FILE "/dev/my_button_device"
#define UP_ARROW  1
#define LEFT_ARROW  2
#define RIGHT_ARROW  3
#define DOWN_ARROW 4
#define ENTER      5

int Button_OpenDevFile();
int Button_read(int fdt, char *buff, size_t size) ;
int Button_check_any_press(int fd, char *buff, size_t size) ;
int Button_press(char *buff);
int Button_waitForAnyKey(int fd, char *buff, size_t size) ;
int Button_KeysPressed_to_Direction(int fd, char *buff, size_t size, int direction);

#endif