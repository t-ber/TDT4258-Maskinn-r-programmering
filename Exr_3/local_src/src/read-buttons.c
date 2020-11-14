#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/flags.h>

#include "../inc/read-buttons.h"
#include "../inc/snake.h"

static FILE *driver_file;

void setup_driver()
{
    // Open driver

    driver_file = fopen("dev/gamepad-driver", "rb");

    if ( driver_file == NULL ) {
        printf("game: error opening gamepad-driver.\n");
        exit(1);
    }
    printf("game: opened gamepad-driver.\n");

    // Reqeust Async Notification (signal)

    signal(SIGIO, &signal_handler);
    fcntl(fileno(driver_file), F_SETOWN, getpid());
    int oflags = fcntl(fileno(driver_file), F_GETFL);
    fcntl(fileno(driver_file), F_SETFL, oflags | FASYNC);
    
	printf("game: driver setup completed.\n");
}

void signal_handler(int sig)
{
	printf("game: signal detected.\n");

    if (sig == SIGIO) {
        uint8_t button_status = (uint8_t) getc(driver_file);

        if (button_status & 0x01 && snake_1.direction != 'R') {
            printf("game: button L pressed.\n");
            snake_1.direction = 'L';
        }

        else if (button_status & 0x02 && snake_1.direction != 'D') {
            printf("game: button U pressed.\n");
            snake_1.direction = 'U';
        }

        else if (button_status & 0x04 && snake_1.direction != 'L') {
            printf("game: button R pressed.\n");
            snake_1.direction = 'R';
        }

        else if (button_status & 0x08 && snake_1.direction != 'U') {
            printf("game: button D pressed.\n");
            snake_1.direction = 'D';
        }

		else if (button_status & 0x80) {
			printf("game: button SW8 pressed.\n");
            // Restarte spill ?
		}
    }
}