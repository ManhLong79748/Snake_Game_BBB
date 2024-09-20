
## Description
    This project implements a snake game on the BeagleBone Black board, displayed on an OLED screen SSD1306.

## Hardware Configuration
    - SSD1306_SCL:  Connected to P9_19
    - SSD1306_SDA:  Connected to P9_20
    - SSD1306_GND:  Connected to P9_01 
    - SSD1306_VCC:  Connected to P9_07
    - Up Button:    Connected to P9_02 and P8_13
    - Down Button:  Connected to P9_02 and P8_10
    - Left Button:  Connected to P9_02 and P8_12
    - Right Button: Connected to P9_02 and P8_11
    - Enter Button: Connected to P9_02 and P8_09

## Instructions
    Use a Makefile to build the driver ssd1306_oled_driver.ko, driver_button.ko, and the main program
    insmod ssd1306_lcd_driver.ko
    insmod driver_button.ko
    ./main to run 

## Demo
    https://youtu.be/coXRShT3gcE?si=idPLl5EPTqryPUT8