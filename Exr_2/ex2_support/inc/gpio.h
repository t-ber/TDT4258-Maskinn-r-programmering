#ifndef GPIO_H
#define GPIO_H
#define REG_LED_OFFSET 7

void setupGPIO();

/*Turns on the specified led*/
void turnOnLed(uint8_t led_num);

/*Turns off the specified led*/
void turnOffLed(uint8_t led_num);

/*Toggles the specified led*/
void toggleLed(uint8_t led_num);

/*Returns 1 if the specified buttons is pressed, 0 otherwise*/
uint8_t btnIsPressed(uint8_t numButton);
#endif