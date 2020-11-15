#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "read-buttons.h"
#include "snake.h"

static FILE *driver_file;

void setup_driver()
{
    // Open driver

    driver_file = fopen("/dev/gamepad_driver", "rb");

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
        char button = (char) 0;

        if (button_status & 0x01) {
            printf("game: button L pressed.\n");
            button = 'L';
        }

        else if (button_status & 0x02) {
            printf("game: button U pressed.\n");
            button = 'U';
        }

        else if (button_status & 0x04) {
            printf("game: button R pressed.\n");
            button = 'R';
        }

        else if (button_status & 0x08) {
            printf("game: button D pressed.\n");
            button = 'D';
        }

		else if (button_status & 0x80) {
			printf("game: button d pressed.\n");
            button = 'd';
		}

        if (button != (char) 0) {
            on_button_pressed(button);
        }
    }
}