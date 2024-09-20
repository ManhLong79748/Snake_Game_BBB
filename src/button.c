#include "button.h"


int Button_OpenDevFile()
{
    int fd = open(SSD1306_BUTTON_FILE, O_RDONLY | O_NONBLOCK);
    if (fd == -1) {
        perror("Failed to open device file");
        exit(EXIT_FAILURE);
        return 0;
    }

    return fd;
}


int Button_read(int fdt, char *buff, size_t size) {
    fd_set fds;
    struct timeval timeout;
    int re;

    FD_ZERO(&fds);
    FD_SET(fdt, &fds);

    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    if (select(fdt + 1, &fds, NULL, NULL, &timeout) > 0) {
        re = read(fdt, buff, size - 1);
    } else {
        re = -1; 
    }

    return re;
}

// Check if any button is pressed
int Button_check_any_press(int fd, char *buff, size_t size) {
    int ret = Button_read(fd, buff, size);

    if (ret <= 0) {
        return 0;
    } else {
        return 1;
    }
}

// Get the pressed button
int Button_press(char *buff) {
    return atoi(buff);
}

// Wait until any button is pressed and release it
int Button_waitForAnyKey(int fd, char *buff, size_t size) {
    int pressed;

    while (!Button_check_any_press(fd, buff, size));

    pressed = Button_press(buff);
    return pressed;
}

//check keypress and return direction
int Button_KeysPressed_to_Direction(int fd, char *buff, size_t size, int direction)
{
    int pressed;
    if(Button_check_any_press(fd, buff, size)) //If a key is pressed
    {
        pressed=Button_press(buff);
        if (direction != pressed)
        {
            if(pressed == DOWN_ARROW && direction != UP_ARROW)
                direction = pressed;
            else if (pressed == UP_ARROW && direction != DOWN_ARROW)
                direction = pressed;
            else if (pressed == LEFT_ARROW && direction != RIGHT_ARROW)
                direction = pressed;
            else if (pressed == RIGHT_ARROW && direction != LEFT_ARROW)
                direction = pressed;
        }
    }
        return(direction);
}