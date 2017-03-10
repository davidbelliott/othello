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
    
    // Constructors / Destructors
    Player(Side side_in);
    ~Player();

    // Mutators
    void set_board(Board* board_in);
    void set_side(Side side_in);

    // Accessors
    Board* get_board();
    Side get_side();

    // Heuristics
    int parity_heuristic(Board* board, Side move_side);
    int edge_heuristic(Board* board, Side move_side);
    int mobility_heuristic(Board* board, Side move_side);
    double heuristic(Board* board, Side move_side);

    // Minimax
    double minimax(Board* board, int depth, bool maximizing, double& a, double& b);

    // Game code
    Move *doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
};
