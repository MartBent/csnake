#include <stdio.h>
#include <stdbool.h>  
#include <time.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "types.h"

//Since the screen is 1024*1024 the width of the rectangles should be 32 pixels. 
#define RES 128 //Resolution should always be square, and a multiple of 32
#define SIZE 32 //Game will always be 32 tiles
#define SQUARE_SIZE RES/SIZE //Adjust the square sizes to the resolution ratio
#define SQUARE_DIAMETER SQUARE_SIZE/2

typedef u8 framebuffer_t[RES][RES];

//Extern
int usleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}

//Extern
direction_t read_direction() {
    
}

//Extern
void flush_framebuffer(const framebuffer_t* buffer) {

}

void clear_buffer(framebuffer_t* buffer) {
    
    for(u8 x = 0; x < RES; x++) {
        for(u8 y = 0; y < RES; y++) {
            *buffer[x][y] = 32;
        }
    }
}

void print_buffer(const framebuffer_t* grid) {
    
    for (u8 i = 0; i < RES; i++) {
        for (u8 j = 0; j < RES; j++) {
            printf("%c", *grid[j][i]);
        }
        printf("\n");
    }
}

void draw_square(framebuffer_t* buffer, point_t location) {
    //Location is valid
    if(location.x < SIZE && location.y < SIZE) {
        for(u8 x = 0; x < RES; x++) {
            for(u8 y = 0; y < RES; y++) {
                if(is_between(location.x*SQUARE_SIZE - SQUARE_DIAMETER, location.x*SQUARE_SIZE  + SQUARE_DIAMETER, x) && 
                   is_between(location.y*SQUARE_SIZE - SQUARE_DIAMETER, location.y*SQUARE_SIZE  + SQUARE_DIAMETER, y)) {
                    *buffer[x][y] = 88;
                }
            }
        }
    }
}

bool detect_collision_snake(const snake_t* snake) {
    for(int i = 1; i < snake->length; i++) {
        if(point_t_equals(snake->current_location, snake->point_history[i])) {
            return true;
        }
    }
    return false;
}

void move_snake(snake_t* snake) {
    switch(snake->current_direction) {
        case down: {
            snake->current_location.y = snake->current_location.y == SIZE ? 1 : snake->current_location.y + 1;
            break;
        }
        case up: {
            snake->current_location.y = snake->current_location.y == 0 ? SIZE-1 : snake->current_location.y - 1;
            break;
        }
        case right: {
            snake->current_location.x = snake->current_location.x == SIZE ? 1 : snake->current_location.x + 1;
            break;
        }
        case left: {
            snake->current_location.x = snake->current_location.x == 0 ? SIZE-1 : snake->current_location.x - 1;
            break;
        }
    }


    memmove(&snake->point_history[1], &snake->point_history[0], (19)*sizeof(point_t));
    snake->point_history[0] = snake->current_location;
}

void game_play() {

    snake_t snake = {
        .current_direction = right,
        .current_location = {1,31},
        .length = 1,
        .point_history = {}
    };
    snake.point_history[0] = snake.current_location;

    point_t current_food = {
        31,
        31
    };

    framebuffer_t* frame_buffer = malloc(sizeof(framebuffer_t[RES][RES]));
    if(frame_buffer == NULL) {
        printf("Malloc failed!");
    }

    while(1) {

        flush_framebuffer(frame_buffer);

        clear_buffer(frame_buffer);

        for(int i = 0; i < snake.length; i++) {
            draw_square(frame_buffer, snake.point_history[i]);
        }

        draw_square(frame_buffer, current_food);

        print_buffer(frame_buffer);
      
        //Move snake
        move_snake(&snake);
        printf("Location: X: %d, Y: %d\n", snake.current_location.x, snake.current_location.y);
        //Detect collision with food / snake
        if(detect_collision_snake(&snake)) {

            //Dead
            printf("Dead\n");
            usleep(10000);  
        }

        if(point_t_equals(snake.current_location, current_food)) {
            snake.length += 1;
            
            current_food.x = rand() % SIZE;
            current_food.y = rand() % SIZE;
        }

        //Process any food
        usleep(100);
    }   
}
int main() {
    srand(time(NULL));
    game_play();
}   
