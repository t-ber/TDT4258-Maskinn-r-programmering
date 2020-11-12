#include "snake.h"

struct Snake snake_is_dead(struct Snake snake){
    if (snake.x_pos == snake.board_size_x || snake.y_pos == snake.board_size_y){
        snake.alaive = 0;
        return snake;
    }
    if (snake.x_pos < 0 || snake.y_pos < 0){
        snake.alaive = 0;
        return snake;
    }
    
    for (int i = 1; i < sizeof(snake.body); i++){
        if (snake.body[0] == snake.body[i]){
            snake.alaive = 0;
            return snake;
        }
    }

    return snake;
}

struct Snake update_snake_head_position(struct Snake snake){
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
    return snake;
}

struct Snake update_snake_body_pos(struct Snake snake){
    for (int i = 1; i < 20; i++){
        if (snake.body[i] != -1) 
            snake.body[i] = snake.body[i - 1];
        else
            return snake;
    }
    return snake;
}

struct Snake head_pos_to_array_pos(struct Snake snake){
    int array_pos = snake.x_pos + snake.y_pos*snake.board_size_x;
    snake.body[0] = array_pos;
    return snake;
}

struct Snake spawn_apple(struct Snake snake){
    time_t t;
    short apple_pos = -1;
    /* Intializes random number generator */
    srand((unsigned) time(&t));

    
    int board_size = snake.board_size_x*snake.board_size_y;
    while(1){
        apple_pos = rand() % board_size;
        for (int i = 0; i < sizeof(snake.body); i++){
            if (snake.body[i] == apple_pos)
                apple_pos = -1;
                break;
        }
        if (apple_pos != -1){
            snake.apple_pos = apple_pos;
            return snake;
        }
    }
   
}

void update_snake_on_board(char *board, struct Snake snake){
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
    
    
}

void clear_board(char *board, int board_size){
    for (int i = 0; i < board_size; i++){
        board[i] = ' ';
    } 
}

struct Snake eat(struct Snake snake, uint8_t last_tail_pos){
    if (snake.body[0] == snake.apple_pos){
        for (int i = 0; i < sizeof(snake.body); i++){
            if (snake.body[i] == -1){
                snake.body[i] = last_tail_pos;
                snake.tail_indx++;
                snake = spawn_apple(snake);
                return snake;
            }
        }
    }
    return snake;
}

struct Snake move_snake(char *board, struct Snake snake){
    uint8_t last_tail_pos = snake.body[snake.tail_indx];
    snake = update_snake_body_pos(snake);
    snake = update_snake_head_position(snake);
    snake = head_pos_to_array_pos(snake);
    snake = snake_is_dead(snake);
    snake = eat(snake, last_tail_pos);
    update_snake_on_board(board, snake);
    
    return snake;
}

void print_board(char *board, int y_size, int x_size){
    
    for (int i = 0; i < y_size; i++){
        for (int j = 0; j < x_size; j++){
            int pos = j + x_size*i;
            printf("[%c]", board[pos]);
        }
        printf("\n");
    }
    printf("\033[5A");
}


void example_with_setup(){
    int board_x = 5;
    int board_y = 5;
    const int BOARD_SIZE = 25;
    char board[25] = {' '};
    for (int i = 0; i < BOARD_SIZE; i++){
        board[i] = ' ';
    }
    
    struct Snake snake_1;  
    snake_1.alaive = 1;
    snake_1.x_pos = 0;
    snake_1.y_pos = 0;
    snake_1.board_size_x = board_x;
    snake_1.board_size_y = board_y;
    snake_1.direction = 'D';
    snake_1.body[0] = 0;
    for (int i = 1; i < 20; i++){
        snake_1.body[i] = -1;
    }
    
    snake_1.tail_indx = 0;

    snake_1.apple_pos = 5;
    update_snake_on_board(board, snake_1);
 
    print_board(board, board_x, board_y);
    clear_board(board, BOARD_SIZE);
    sleep(1);
    while(1){
        snake_1 = move_snake(board, snake_1);
        
        if (!snake_1.alaive){
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
}