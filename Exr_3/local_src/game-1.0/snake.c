#include <stdbool.h>
#include <time.h>
#include <unistd.h>

#include "snake.h"
#include "framebuffer.h"

static struct Snake snake;

void on_button_pressed(char button) {
    switch (button)
    {
        case 'd':
            if (snake.alive) {
                stop_game();
            }
            else {
                run_game();
            }
            break;

        case 'L':
            if (snake.direction != 'R') {
                snake.next_direction = 'L';
            }
            break;

        case 'U':
            if (snake.direction != 'D') {
                snake.next_direction = 'U';
            }
            break;
        
        case 'R':
            if (snake.direction != 'L') {
                snake.next_direction = 'R';
            }
            break;
        
        case 'D':
            if (snake.direction != 'U') {
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
    for (i = 1; i < sizeof(snake.body); i++) {
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
    for (i = 1; i < 20; i++){
        if (snake.body[i] != -1) 
            snake.body[i] = snake.body[i - 1];
        else
            return;
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
        for (i = 0; i < sizeof(snake.body); i++){
            if (snake.body[i] == apple_pos)
                apple_pos = -1;
                break;
        }
        if (apple_pos != -1){
            snake.apple_pos = apple_pos;
            return;
        }
    }
   
}

/* void update_snake_on_board(char *board) {
    uint8_t snake_part_position;
    board[snake.apple_pos] = 'A';
    board[snake.body[0]] = 'H';
    
    for (int i = 1; i < 20; i++){
        if (snake.body[i] != -1 ){
            snake_part_position = snake.body[i];
            board[snake_part_position] = 'S';
        }
        else{
            board[snake.body[i]] = ' ';
        }
        
    }
    
    
} */

/* void clear_board(char *board, int board_size){
    for (int i = 0; i < board_size; i++){
        board[i] = ' ';
    } 
} */

void eat(uint8_t last_tail_pos){
    if (snake.body[0] == snake.apple_pos) {
        int i;
        for (i = 0; i < sizeof(snake.body); i++) {
            if (snake.body[i] == -1){
                snake.body[i] = last_tail_pos;
                snake.tail_indx++;
                spawn_apple();
                return;
            }
        }
    }
    return;
}

void move_snake() {
    snake.direction = snake.next_direction;
    uint8_t last_tail_pos = snake.body[snake.tail_indx];
    update_snake_body_pos();
    update_snake_head_position();
    head_pos_to_array_pos();
    snake_is_dead();
    eat(last_tail_pos);

    printf("y pos of head: %i\n", snake.body[0]/snake.board_size_x);
}

// void print_board(char *board, int y_size, int x_size){
//     int i;
//     int j;
//     for (i = 0; i < y_size; i++){
//         for (j = 0; j < x_size; j++){
//             int pos = j + x_size*i;
//             printf("[%c]", board[pos]);
//         }
//         printf("\n");
//     }
//     printf("\033[5A");
// }

/*void draw_snake() {
    draw_square(snake.x_pos, snake.y_pos,1)
    if(!hit)
    {
        int x_pos = snake.body[snake.tail_indx]%snake.board_size_x;
        int y_pos = (snake.body[snake.tail_indx]-x_pos)/snake.board_size_x;
        draw_square(x_pos, y_pos,0);
    }

    
}*/

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
        draw_square(x_pos,y_pos, 1);
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
    snake.alive = false;
    snake.apple_pos = 200;
    snake.direction = 'D';
    snake.next_direction = 'D';

    snake.body[0] = 0;
    int i;
    for (i = 1; i < 20; i++) {
        snake.body[i] = -1;
    }

    draw_screen();    
}

void run_game()
{
    snake.alive = true;

    while (snake.alive) {
        move_snake();
        draw_screen();
        usleep(100000);
    }
}

void stop_game()
{
    snake.alive = false;
}


/* void example_with_setup() {
    int board_x = 32;
    int board_y = 24;
    const int BOARD_SIZE = 768;
    char board[768] = {' '};
    for (int i = 0; i < BOARD_SIZE; i++){
        board[i] = ' ';
    }
    
    snake.alive = 1;
    snake.x_pos = 0;
    snake.y_pos = 0;
    snake.board_size_x = board_x;
    snake.board_size_y = board_y;
    snake.direction = 'D';
    snake.body[0] = 0;
    for (int i = 1; i < 20; i++){
        snake.body[i] = -1;
    }
    
    snake.tail_indx = 0;

    snake.apple_pos = 5;
    update_snake_on_board(board, snake_1);
 
    print_board(board, board_x, board_y);
    clear_board(board, BOARD_SIZE);
    sleep(1);
    while(1){
        move_snake(board, snake_1);
        
        if (!snake.alive){
            printf("dead");
            break;
        }
       
        //snake_1 = update_snake_head_position(snake_1);
        //snake_1 = update_snake_body_pos(snake_1);
        //update_snake_on_board(board, snake_1);
        //printf("%i\n",snake_1.body[0]);
        //printf("%c", board[snake_1.body[0]]);
        print_board(board, board_x, board_y);
        clear_board(board, BOARD_SIZE);
        sleep(1);
    }
} */

