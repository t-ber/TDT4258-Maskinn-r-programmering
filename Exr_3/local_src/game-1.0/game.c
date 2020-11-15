#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

	printf("Hello World, I'm game!\n");
	initialize_screen();
	setup_driver();
	snake_reset();


	/*while(1){
		move_snake();
		if(!snake.alive){
			printf("Dead");
			break;
		}
		sleep();
	}
	exit(EXIT_SUCCESS);*/
}
