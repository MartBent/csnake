#include "portable-snake/snake.h"

void delay(unsigned long msec) {
    //This function should block for the given amount of milliseconds.
}

void display_frame(const u8* grid, const unsigned long resolution) {
    //Parameter grid will hold a pointer to a resolution*resolution sized grid. This grid holds the current state of the snake game.
}

void display_score(u8 score) {
   //Parameter score will hold the current length of the snake. 
}

u8 rnd() {
    //Generate a random number between 0 and 32.
}

direction_t read_direction() {
    //This function should return the direction of the snake, depending on a user implemented input.
    return down;
}


int main() {
    snake_driver_t driver;
    driver.delay_function_cb = delay;
    driver.display_frame_cb = display_frame;
    driver.display_score_cb = display_score;
    driver.random_number_cb = rnd;
    driver.read_direction_cb = read_direction;
    driver.resolution = 32;
    driver.snake_length = 33;
    driver.frame_buffer = malloc(driver.resolution*driver.resolution);
    snake_play(&driver);
}   