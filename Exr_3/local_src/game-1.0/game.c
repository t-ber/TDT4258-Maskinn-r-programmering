#include <stdio.h>
#include <stdlib.h>

#include "framebuffer.h"
#include "snake.h"
#include "read-buttons.h"

int main(int argc, char *argv[])
{

	printf("Hello World, I'm game!\n");
	initialize_screen();
	setup_driver();
	snake_reset();

	int pres = -1;
	while (pres == -1) {
		pres = pause();
	}

	return 0;
}
