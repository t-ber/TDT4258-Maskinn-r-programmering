#ifndef SNAKE_H
#define SNAKE_H

#include <stdio.h>
#include <stdint.h>
#include <unistd.h> //for sleep
#include <stdlib.h> //for rand

struct Snake
{   
    uint8_t board_size_x;
    uint8_t board_size_y;
    uint8_t x_pos;
    uint8_t y_pos;
    int8_t tail_indx;
    int8_t body[20];
    int8_t alaive;
    short apple_pos;
    char direction;
};

/*Checks if the snake is dead*/
struct Snake snake_is_dead(struct Snake snake);

/*Increments the x or y position of the snake head*/
struct Snake update_snake_head_position(struct Snake snake);

/*Transforms the x and y pos of the snake head into the 1D board position*/
struct Snake head_pos_to_array_pos(struct Snake snake);

/*Updates the 1D board positions in the snake.body array*/
struct Snake update_snake_body_pos(struct Snake snake);

/*Spawns an apple on the board somwere the snake is not*/
struct Snake spawn_apple(struct Snake snake);

/*Visualises the snake and apples on the board*/
void update_snake_on_board(char *board, struct Snake snake);

/*Clears the board*/
void clear_board(char *board, int board_size);

/*Checks if the snake is eating an apple, if so adds new tail part*/
struct Snake eat(struct Snake snake, uint8_t last_tail_pos);

/*One iteration of moving the snake one space and interacting with the boardstate*/
struct Snake move_snake(char *board, struct Snake snake);

/*Prints the boardstate*/
void print_board(char *board, int y_size, int x_size);

#endif