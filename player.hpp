#pragma once

#include "common.hpp"
#include "board.hpp"
#include <iostream>

using namespace std;

class Player {
private:
    Board* board;
    Side side;

public:
    Player(Side side_in);
    ~Player();

    int greedy_heuristic(Move *move, Side move_side);

    Move *doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
};
