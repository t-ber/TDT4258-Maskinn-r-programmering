#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/flags.h>

#include "../read-buttons.h"
#include "../inc/snake.h"

static FILE driver_file;

void setup_driver()
{
    // Open driver

    driver_file = fopen("dev/gamepad-driver", 'rb');

    if ( driver_file == -1 ) {
        printf("Error opening gamepad-driver.");
        exit(1);
    }
    printf("Opened gamepad-driver.");

    // Reqeust Async Notification (signal)

    signal(SIGIO, &signal_handler);
    fcntl(fileno(driver_file), F_SETOWN, getpid());
    int oflags = fcntl(fileno(driver_file), F_GETFL);
    fcntl(fileno(driver_file), F_SETFL, oflags | FASYNC);
    
}

void signal_handler(int sig)
{
    if (sig == SIGIO) {
        uint8_t button_status = (uint8_t) getc(driver_file);

        if (button_status & 0x01 && snake_1.direction != 'R') {
            snake_1.direction = 'L';
        }

        else if (button_status & 0x02 && snake_1.direction != 'D') {
            snake_1.direction = 'U';
        }

        else if (button_status & 0x04 && snake_1.direction != 'L') {
            snake_1.direction = 'R';
        }

        else if (button_status & 0x08 && snake_1.direction != 'U') {
            snake_1.direction = 'D';
        }
    }
}