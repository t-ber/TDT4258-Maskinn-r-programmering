#ifndef SNAKE_H
#define SNAKE_H

#define BOARD_SIZE_X 32
#define BOARD_SIZE_Y 24
#define BOARD_SIZE 768

#define SNAKE_MAX_LENGTH 768
#define SNAKE_INTERVAL 50000 // microseconds

struct Snake
{   
    uint8_t board_size_x;
    uint8_t board_size_y;
    int8_t x_pos;
    int8_t y_pos;
    uint16_t tail_indx;
    uint32_t body[SNAKE_MAX_LENGTH];
    bool alive;
    bool game_running;
    uint32_t apple_pos;
    char direction;
    char next_direction;
};

/* setup / resetting of the snake, starts the game */
void snake_reset();

/*Checks if the snake is dead*/
void snake_is_dead();

/*Increments the x or y position of the snake head*/
void update_snake_head_position();

/*Transforms the x and y pos of the snake head into the 1D board position*/
void head_pos_to_array_pos();

/*Updates the 1D board positions in the snake.body array*/
void update_snake_body_pos();

/*Spawns an apple on the board somwere the snake is not*/
void spawn_apple();

/*Visualises the snake and apples on the board*/
// void update_snake_on_board(char *board);

/*Clears the board*/
// void clear_board(char *board, int board_size);

/*Checks if the snake is eating an apple, if so adds new tail part*/
void eat(uint32_t last_tail_pos);

/*One iteration of moving the snake one space and interacting with the boardstate*/
void move_snake();

/*Prints the boardstate*/
// void print_board(char *board, int y_size, int x_size);

/*Handles a button press*/
void on_button_pressed(char button);

/*Draws an apple on the screen*/
void draw_apple();

/*Draws the screen based on the snake*/
void draw_screen();

/* Runs the game */
void run_game();

/* Stops the game */
void stop_game();

/* Starts the game */
void start_game();

#endif
