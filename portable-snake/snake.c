#ifndef SNAKE_C
#define SNAKE_C

#include "snake.h"

void clear_buffer(const snake_driver_t* driver, framebuffer_t* buffer) {
    for(u8 x = 0; x < driver->resolution; x++) {
        for(u8 y = 0; y < driver->resolution; y++) {
            *buffer[x][y] = 32;
        }
    }
}

void draw_square(const snake_driver_t* driver, framebuffer_t* buffer, point_t location) {
    //Location is valid
    const unsigned int square_size = driver->resolution / 32;
    const unsigned int square_diameter = square_size / 2;

    if(location.x < SIZE && location.y < SIZE) {
        for(u8 x = 0; x < driver->resolution; x++) {
            for(u8 y = 0; y < driver->resolution; y++) {
                if(is_between(location.x*square_size - square_diameter, location.x*square_size  + square_diameter, x) && 
                   is_between(location.y*square_size - square_diameter, location.y*square_size  + square_diameter, y)) {
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

void move_snake(const snake_driver_t* driver, snake_t* snake) {
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

    memmove(&snake->point_history[1], &snake->point_history[0], (driver->snake_length-1)*sizeof(point_t));
    snake->point_history[0] = snake->current_location;
}

void snake_play(const snake_driver_t* driver) {
    
    snake_t snake = {
        .current_direction = right,
        .current_location = {16,16},
        .length = 1,
        .point_history = {}
    };
    snake.point_history[0] = snake.current_location;

    point_t current_food = {
        driver->random_number_cb(),
        driver->random_number_cb()
    };

    framebuffer_t* frame_buffer = malloc(sizeof(framebuffer_t[driver->snake_length][driver->snake_length]));
    if(frame_buffer == (void*)0) {
       return;
    }

    while(1) {

        snake.current_direction = driver->read_direction_cb();

        clear_buffer(driver, frame_buffer);

        for(int i = 0; i < snake.length; i++) {
            draw_square(driver, frame_buffer, snake.point_history[i]);
        }
        draw_square(driver, frame_buffer, current_food);

        driver->display_flush_cb(frame_buffer, driver->resolution);

        //Move snake
        move_snake(driver, &snake);

        //Detect collision with food / snake
        if(detect_collision_snake(&snake)) {
            driver->delay_function_cb(10000);
        }

        if(point_t_equals(snake.current_location, current_food)) {
            snake.length += 1;
            current_food.x = driver->random_number_cb();
            current_food.y = driver->random_number_cb();
        }

        //Process any food
        driver->delay_function_cb(200);
    }   
}

#endif