#pragma once

#ifndef SSD1306_H
#define SSD1306_H

#include "button.h"


int OLED_OpenDevFile();
void OLED_SetCursor(int fd, int x, int y);
void OLED_Display(int fd, char *str);
void OLED_Clear(int fd);

#endif