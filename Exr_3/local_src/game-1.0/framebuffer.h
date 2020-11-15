#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>
#include <linux/fb.h>
#include <sys/ioctl.h>


#define BLACK	0x0000
#define GREEN	0x37E0
#define RED		0xF800

#define BLOCK_SIZE 10



int initialize_screen();
void clear_screen();
void blackout_screen();
void update_screen();
void draw_square(int pos_x, int pos_y, int color_val);
void draw_test(int pos_x, int pos_y);