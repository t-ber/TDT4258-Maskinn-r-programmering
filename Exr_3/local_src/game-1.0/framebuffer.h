#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#define BLACK       0b0000000000000000
#define GREEN       0b0000011111100000
#define DARK_GREEN  0b0000011001100000
#define RED	        0b1111100000000000

#define BLOCK_SIZE 10

int initialize_screen();
void clear_screen();
void blackout_screen();
void update_screen();
void draw_square(int pos_x, int pos_y, int color_val);
void draw_test(int pos_x, int pos_y);

#endif