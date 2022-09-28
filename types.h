#include <stdint.h>

typedef uint8_t u8;

typedef struct {
    u8 x;
    u8 y;
} point_t;

typedef enum {
    up,
    down,
    left,
    right
}direction_t;

typedef struct {
    point_t current_location;
    direction_t current_direction;
    unsigned int length;
    point_t point_history[20];
    
} snake_t;

typedef struct {
    snake_t snake;
    point_t current_food;
} game_t;

bool point_t_equals(const point_t rhs, const point_t lhs) {
    return (rhs.x == lhs.x) && (rhs.y == lhs.y);
}

bool is_between(u8 min, u8 max, u8 val) {
    return ((val < max) && (val > min)) || val == min;
}