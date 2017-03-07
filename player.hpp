#pragma once

#include "common.hpp"
#include "board.hpp"
#include <iostream>
#include <vector>

using namespace std;

class Player {
private:
    Board* board;
    Side side;

public:
    Player(Side side_in);
    ~Player();

    double minimax(Board* board, int depth, bool maximizing);

    int parity_heuristic(Board* board, Side move_side);
    int edge_heuristic(Board* board, Side move_side);
    double heuristic(Board* board, Side move_side);

//    std::pair<double, Move*> minimax(Board* board, int depth, Side max_side, bool maximizing);

    Move *doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
};
