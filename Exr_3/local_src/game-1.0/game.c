#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "framebuffer.h"
#include "snake.h"
#include "read-buttons.h"

int main(int argc, char *argv[])
{

	printf("Hello World, I'm game!\n");
	initialize_screen();
	setup_driver();
	snake_reset();

	while (true) {
		pause();
		run_game();
	}

	return 0;
}
