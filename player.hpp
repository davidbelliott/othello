#pragma once

#include "common.hpp"
#include "board.hpp"
#include "othello_node.hpp"
#include <iostream>
#include <vector>

using namespace std;

class Player {
private:
    Board* board;
    Side side;
    OthelloNode* root;

public:
    Player(Side side_in);
    ~Player();


//    std::pair<double, Move*> minimax(Board* board, int depth, Side max_side, bool maximizing);

    Move *doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
};
