#include "button.h"
#include "ssd1306.h"
#include "snake.h"

int fd_ssd, fd_button;
char kernel_data[2];

// Clear the terminal
void clrscr() {
    system("clear");
    return;
}

int main() {
    int c;

    fd_ssd = OLED_OpenDevFile();

    fd_button = Button_OpenDevFile();
    clrscr();

    OLED_Clear(fd_ssd);

    do {
        Snake_loadGame(fd_ssd, fd_button, kernel_data, sizeof(kernel_data));
        OLED_Clear(fd_ssd);
        OLED_SetCursor(fd_ssd, 25, 2);
        OLED_Display(fd_ssd, "PLAY AGAIN ?");
        OLED_SetCursor(fd_ssd, 0, 3);
        OLED_Display(fd_ssd, "ENTER. YES  Other. NO");
        c = Button_waitForAnyKey(fd_button, kernel_data, sizeof(kernel_data));
        if (c == 5) {
            OLED_Clear(fd_ssd);
        }
    } while (c == 5);

    OLED_Clear(fd_ssd);
    OLED_SetCursor(fd_ssd, 30, 3);
    OLED_Display(fd_ssd, " END GAME !");

    return 0;
}
