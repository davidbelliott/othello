#pragma once

static const char WHITE = 'w';
static const char BLACK = 'b';

#define OTHER_SIDE(side)    \
    (side == WHITE ? BLACK : WHITE)

#define INFINITY std::numeric_limits<int>::max()

class Move {
   
public:
    int x, y;

    Move(int x_in = 0, int y_in = 0)
        : x(x_in),
          y(y_in)
    { }

    ~Move() {}

    int get_x() { return x; }
    int get_y() { return y; }

    void set_x(int x_in) { x = x_in; }
    void set_y(int y_in) { y = y_in; }
};
