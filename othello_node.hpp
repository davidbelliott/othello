#pragma once
#include "board.hpp"
#include "common.hpp"
#include <vector>

class OthelloNode
{
    public:
        Board* board;
        Side side;
        double heuristic;

        std::vector<OthelloNode*> children;

        OthelloNode(Board* board_in, Side side_in);
        ~OthelloNode();

        void populate_children(int depth);

        int parity_heuristic(Side move_side);
        int edge_heuristic(Side move_side);
        double heuristic(Side move_side);
};
