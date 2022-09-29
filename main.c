#include <time.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

#include "portable-snake/snake.h"

void delay(unsigned long msec)
{
    struct timespec ts;
    int res;

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);
}

void display_frame(const u8* grid, const unsigned long resolution) {
    
    for (u8 i = 0; i < resolution; i++) {
        printf("-");
    }
    printf("\n");
    for (u8 i = 0; i < resolution; i++) {
        printf("|");
        for (u8 j = 0; j < resolution; j++) {
            printf("%c", *(u8*)(grid+(resolution*i)+j));
        }
        printf("|");
        printf("\n");
    }
    for (u8 i = 0; i < resolution; i++) {
        printf("-");
    }
    printf("\n");
}
void display_score(u8 score) {
    
}

u8 rnd() {
    u8 res = rand() % 32;
    return res;
}
direction_t read_direction() {
    return down;
}


int main() {
    srand(time(NULL));
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
