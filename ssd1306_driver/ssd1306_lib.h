#ifndef SSD1306_LIB_H
#define SSD1306_LIB_H

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/slab.h> 
#include <linux/fs.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/i2c.h>
#include <linux/uaccess.h>
#include <linux/errno.h>
#include <linux/delay.h>


#define SSD1306_MAX_SEG 128
#define SSD1306_MAX_LINE 7
#define SSD1306_DEF_FONT_SIZE 5

struct ssd1306_i2c_module {
	struct i2c_client *client;
	uint8_t line_num;
	uint8_t cursor_position;
	uint8_t font_size;
};

int ssd1306_i2c_send(struct ssd1306_i2c_module *module , char *buff, int len);
void ssd1306_write(struct ssd1306_i2c_module *module, bool check, char data);
void ssd1306_set_cursor(struct ssd1306_i2c_module *module, uint8_t line_num, uint8_t cursor_position);

void  ssd1306_goto_next_line(struct ssd1306_i2c_module *module);

int convert(char c);

void ssd1306_print_char(struct ssd1306_i2c_module *module, unsigned char c);

void ssd1306_print_string(struct ssd1306_i2c_module *module, unsigned char *str);

void ssd1306_set_brightness(struct ssd1306_i2c_module *module, uint8_t brightness);

void ssd1306_clear_page(struct ssd1306_i2c_module *module, uint8_t line);
void ssd1306_clear_full(struct ssd1306_i2c_module *module);
int ssd1306_display_init(struct ssd1306_i2c_module *module);


#endif