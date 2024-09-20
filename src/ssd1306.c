#include "ssd1306.h"

#define SSD1306_DEV_FILE    "/dev/my_device_ssd"


int OLED_OpenDevFile(){
    int fd = open(SSD1306_DEV_FILE, O_WRONLY);

    if (fd == -1) {
        printf("Open /dev/my_ssd1306 failed. Please check the dev folder\n");
        exit(EXIT_FAILURE);
    }
    return fd;
}

//set cursor locate in column x, line y to ssd1306
void OLED_SetCursor(int fd, int x, int y)
{
    char str[20];
    sprintf(str, "cursor %d %d", x, y);
    int w = write(fd, str, strlen(str));   
    if ( w == -1)
    {
        printf("Can not set cursor to LCD\n");
    }
}

//Display str to ssd1306
void OLED_Display(int fd, char *str){
    int w = write(fd, str, strlen(str));   
    if ( w == -1)
    {
        printf("Can not write to LCD\n");
    }
}

//clear ssd screen
void OLED_Clear(int fd)
{
    write(fd, "clear", 5);
}


