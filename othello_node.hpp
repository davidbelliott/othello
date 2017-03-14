#pragma once
#include "board.hpp"
#include "common.hpp"
#include <map>

struct OthelloNode
{
    Board* board;
    Side move_side;
    double weight;

    Move best_move;
    std::map<Move, OthelloNode*> children;

    OthelloNode(Board* board_in, Side move_side_in);
    ~OthelloNode();
};
