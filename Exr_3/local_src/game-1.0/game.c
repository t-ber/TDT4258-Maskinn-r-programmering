#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
// #include <linux/flags.h>
#include <unistd.h>


//framebuffer.h begin



#include <fcntl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <sys/ioctl.h>


#define BLACK	0x0000
#define GREEN	0x37E0
#define RED		0xF800

#define BLOCK_SIZE 10
//framebuffer.h end

//framebuffer.c begin
int fbfd;
int pos_x;
int pos_y;
int screensize;
uint16_t* screen;
struct fb_copyarea rect;
static uint16_t colors[] = {BLACK, GREEN, RED};


int initialize_screen()
{
	printf("initialize screen\n");
	
	fbfd = open("dev/fb0", O_RDWR);
	if(fbfd == -1)
	{
		printf("open error\n");
		exit(1);
	}
	//printf("opened file");
	
	
	rect.dx = 0;
	rect.dy = 0;
	rect.width = 320;
	rect.height = 240;
	
	//printf("setting screen settings\n");
	
	screensize = rect.width*rect.height*2;
	//printf("Calculating screensize\n");
	
	screen =(uint16_t*) mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	//printf("Mapped screen to memory\n");
	if(screen == MAP_FAILED)
	{
	//printf("mapping screen error\n");
	close(fbfd);
	exit(1);
	}
	printf("Finished initialize\n");
	
	return 1;
}

void clear_screen()
{
	printf("clear screen\n");
	munmap(screen, screensize);
	close(fbfd);
}

void clear_graphics()
{
	int i;
	for(i = 0; i<screensize; i++)
	{
		screen[i] = colors[0];
	}
}


void update_screen()
{
	printf("update screen\n");
	ioctl(fbfd, 0x4680, &rect);
}

void draw_square(int pos_x, int pos_y, int color_val)
{
	int i;
	int j;
	int pos_x_scaled = pos_x*BLOCK_SIZE;
	int pos_y_scaled = pos_y*BLOCK_SIZE;
	printf("draw square\n");
	//printf("screen size: %i", screensize);
	for(i=0; i<BLOCK_SIZE;i++)
	{
		for(j=0; j<BLOCK_SIZE; j++)
		{
			//printf("i: %i\n", i);
			//printf("j: %i\n", j);
			//printf("pixel:  %i\n", j+i*rect.width);
			
			screen[pos_x_scaled+j+(i+pos_y_scaled)*rect.width] = colors[color_val];
		}
	

	}

}
//framebuffer.c end


static FILE *driver_file;

void setup_driver();
void signal_handler(int sig);

int main(int argc, char *argv[])
{
	printf("game: hello World, I'm game!\n");
	initialize_screen();
	setup_driver();
	
	int pres = -1;
	while (pres == -1) {
		pres = pause();
	}

	exit(EXIT_SUCCESS);
}


void setup_driver()
{
    // Open driver

    driver_file = fopen("/dev/gamepad_driver", "rb");

    if ( driver_file == NULL ) {
        printf("game: error opening gamepad-driver.");
        exit(1);
    }
    printf("game: opened gamepad-driver.");

    // Reqeust Async Notification (signal)

    signal(SIGIO, &signal_handler);
    fcntl(fileno(driver_file), F_SETOWN, getpid());
    int oflags = fcntl(fileno(driver_file), F_GETFL);
    fcntl(fileno(driver_file), F_SETFL, oflags | FASYNC);
    
	printf("game: driver setup completed.");
}

void signal_handler(int sig)
{
	printf("game: signal detected.");

    if (sig == SIGIO) {
        uint8_t button_status = (uint8_t) getc(driver_file);
        //uint8_t button_status = 0x01;

        if (button_status & 0x01) {
            printf("game: button L pressed.");
            draw_square(5,5,2);
        }

        else if (button_status & 0x02) {
            printf("game: button U pressed.");
            draw_square(5,5,1);
        }

        else if (button_status & 0x04) {
            printf("game: button R pressed.");
            draw_square(6,6,2);
        }

        else if (button_status & 0x08) {
            printf("game: button D pressed.");
            clear_graphics();
        }
        update_screen();
    }
}
