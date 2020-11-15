#include "framebuffer.h"

int fbfd;
int pos_x;
int pos_y;
int i;
int j;
int screensize;
uint16_t* screen;
struct fb_copyarea rect;
static uint16_t colors[] = {BLACK, GREEN, RED, DARK_GREEN};


int initialize_screen()
{
	printf("initialize screen\n");
	
	fbfd = open("dev/fb0", O_RDWR);
	if(fbfd == -1)
	{
		printf("open error\n");
		exit(1);
	}
	printf("opened file");
	
	
	rect.dx = 0;
	rect.dy = 0;
	rect.width = 320;
	rect.height = 240;
	
	printf("setting screen settings\n");
	
	screensize = rect.width*rect.height*2;
	printf("Calculating screensize\n");
	
	screen =(uint16_t*) mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	printf("Mapped screen to memory\n");
	if(screen == MAP_FAILED)
	{
	printf("mapping screen error\n");
	close(fbfd);
	exit(1);
	}
	printf("Finished initialize\n");
	
	return 1;
}

void clear_screen()
{
	munmap(screen, screensize);
	close(fbfd);
}

void blackout_screen()
{
	int i;
    for(i = 0; i < screensize; i++) {
        screen[i] = colors[0];
    }
}


void update_screen()
{
	ioctl(fbfd, 0x4680, &rect);
}

void draw_square(int pos_x, int pos_y, int color_val)
{
	for(i=0; i<BLOCK_SIZE;i++)
	{
		for(j=0; j<BLOCK_SIZE; j++)
		{
			screen[pos_x+j+(i+pos_y)*rect.width] = colors[color_val];
		}
	}

}


void example(int pos_x, int pos_y)
{
    initialize_screen();
    draw_square(10,10,1);
    update_screen();

}


