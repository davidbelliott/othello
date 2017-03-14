#pragma once
#include "board.hpp"
#include "common.hpp"
#include <map>

struct OthelloNode
{
    char move_side;
    bool best_move_exists;
    Move best_move;
    int depth_checked;
};
