#ifndef HEADER_GPIO
#define HEADER_GPIO

void gpio_init();
void wait_button_click();
int read_button();
void red(int on);
void blue(int on);
void green(int on);

#endif
