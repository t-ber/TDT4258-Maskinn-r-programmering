#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

#include "snake.h"
#include "framebuffer.h"

static struct Snake snake;

void on_button_pressed(char button) {
    switch (button)
    {
        case 'u':
            if (snake.game_running) {
                stop_game();
            }
            else if (snake.alive) {
                start_game();
            }
            break;

        case 'd':
            snake_reset();
            break;

        case 'L':
            if (snake.alive && snake.direction != 'R') {
                snake.next_direction = 'L';
            }
            break;

        case 'U':
            if (snake.alive && snake.direction != 'D') {
                snake.next_direction = 'U';
            }
            break;
        
        case 'R':
            if (snake.alive && snake.direction != 'L') {
                snake.next_direction = 'R';
            }
            break;
        
        case 'D':
            if (snake.alive && snake.direction != 'U') {
                snake.next_direction = 'D';
            }
            break;
        
        default:
            break;
    }
}

void snake_is_dead() {
    if (snake.x_pos == snake.board_size_x || snake.y_pos == snake.board_size_y){
        snake.alive = false;
        return;
    }
    if (snake.x_pos < 0 || snake.y_pos < 0){
        snake.alive = false;
        return;
    }
    
    int i;
    for (i = 1; i < SNAKE_MAX_LENGTH; i++) {
        if (snake.body[0] == snake.body[i]) {
            snake.alive = false;
            return;
        }
    }

}

void update_snake_head_position() {
    switch (snake.direction)
    {
    case 'R':
        snake.x_pos++;
        break;
    
    case 'L':
        snake.x_pos--;
        break;
    
    case 'U':
        snake.y_pos--;
        break;

    case 'D':
        snake.y_pos++;
    default:
        break;
    }
}

void update_snake_body_pos() {
    int i;
    for (i = snake.tail_indx; i > 0; i--) {
        if (snake.body[i] != -1) {
            snake.body[i] = snake.body[i - 1];
        }
    }
    return;
}

void head_pos_to_array_pos() {
    int array_pos = snake.x_pos + snake.y_pos*snake.board_size_x;
    snake.body[0] = array_pos;
    return;
}

void spawn_apple() {
    time_t t;
    short apple_pos = -1;
    /* Intializes random number generator */
    srand((unsigned) time(&t));

    
    int board_size = snake.board_size_x*snake.board_size_y;
    while(1){
        apple_pos = rand() % board_size;
        int i;
        for (i = 0; i <= snake.tail_indx; i++) {
            if (snake.body[i] == apple_pos) {
                apple_pos = -1;
                break;
            }
        }
        if (apple_pos != -1) {
            snake.apple_pos = apple_pos;
            return;
        }
    }
   
}

void eat(uint32_t last_tail_pos) {
    if (snake.body[0] == snake.apple_pos) {
        if (snake.tail_indx < SNAKE_MAX_LENGTH - 1) {
            snake.tail_indx++;
            snake.body[snake.tail_indx] = last_tail_pos;
        }
        spawn_apple();
    }
}

void move_snake() {
    snake.direction = snake.next_direction;
    uint32_t last_tail_pos = snake.body[snake.tail_indx];
    update_snake_body_pos();
    update_snake_head_position();
    head_pos_to_array_pos();
    snake_is_dead();
    eat(last_tail_pos);
}

void draw_apple()
{
        int x_pos = snake.apple_pos % snake.board_size_x * BLOCK_SIZE;
        int y_pos = snake.apple_pos / snake.board_size_x * BLOCK_SIZE;
        draw_square(x_pos, y_pos, 2);
}

void draw_screen(){
    blackout_screen();
    int j = 0;
    while (snake.body[j] != -1) {
        int x_pos = (snake.body[j]%snake.board_size_x) * BLOCK_SIZE;
        int y_pos = (snake.body[j]/snake.board_size_x) * BLOCK_SIZE;
        if (j == 0) {
            draw_square(x_pos,y_pos, 3);
        }
        else {
            draw_square(x_pos,y_pos, 1);
        }
        j++;
    }
    draw_apple();
    update_screen();
}

void snake_reset()
{
    snake.board_size_x = BOARD_SIZE_X;
    snake.board_size_y = BOARD_SIZE_Y;
    snake.x_pos = 0;
    snake.y_pos = 0;
    snake.tail_indx = 0;
    snake.alive = true;
    snake.game_running = false;
    snake.apple_pos = 200;
    snake.direction = 'D';
    snake.next_direction = 'D';

    snake.body[0] = 0;
    int i;
    for (i = 1; i < SNAKE_MAX_LENGTH; i++) {
        snake.body[i] = -1;
    }

    draw_screen();    
}

void run_game()
{
    if (!snake.game_running) {
        return;
    }

    while (snake.game_running && snake.alive) {
        move_snake();
        draw_screen();
        usleep(SNAKE_INTERVAL);
    }

    printf("snake: snake is dead.\n");
}

void stop_game()
{
    snake.game_running = false;
}

void start_game()
{
    snake.game_running = true;
}
