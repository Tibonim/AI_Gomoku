#ifndef CASE_HPP
#define CASE_HPP
#include <iostream>
# define SIZE 19
# define COLOR_MASK 0x3
# define PUT_MASK 0x4
# define TAKE_MASK 0x8
# define CHECK_TURN(r) ((r) ? Color::black : Color::white)
# define GET_COLOR_WXY(case) (case.property & COLOR_MASK)
# define GET_COLOR(x,y,map) (map[x + (SIZE * y)].property & COLOR_MASK)
# define CAN_PUT_WXY(case) (((case.property & PUT_MASK) >> 2 == 1) ? true : false)
# define CAN_PUT(x,y,map) ((map[x + (SIZE * y)].property & PUT_MASK) >> 2 == 1) ? true : false)
# define CAN_TAKE_WXY(case) (((case.property & TAKE_MASK) >> 3 == 1) ? true : false)
# define CAN_TAKE(x,y,map) ((map[x + (SIZE * y)].property & TAKE_MASK) >> 3 == 1) ? true : false)
# define IS_IN_MAP(x,y) ((x < 0 || x >= SIZE || y < 0 || y >= SIZE) ? false : true)
# define IS_OCCUPIED(c) (c.color == 1 || c.color == 2)
# define INVERT_C(c) ((c == 2) ? Color::white : Color::black)
# define IS_EATABLE_ISLAND(isl) ((isl)->GetBody().size() == 2 && IS_OCCUPIED((isl)->GetBefore()[0]) == !IS_OCCUPIED((isl)->GetAfter()[0]))

enum Color {
    empty = 0,
    white = 1,
    black = 2,
    illegal = 3
};

struct Case {
    unsigned int property : 4;
};

struct Coord {
    unsigned int x : 5;
    unsigned int y : 5;
    unsigned int color : 2;
};

#endif /* !MAP_HPP */
