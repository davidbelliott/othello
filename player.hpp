#pragma once

#include "common.hpp"
#include "board.hpp"
#include <iostream>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;

class Player {
private:
    Board board;
    Side side;
    static const int weights[8][8];
    milliseconds last_move_time;


public:
    
    // Constructors / Destructors
    Player(Side side_in);
    ~Player();

    // Heuristics
    int parity_heuristic(Board* board, Side move_side);
    int corner_heuristic(Board* board, Side move_side);
    int stability_heuristic(Board* board, Side move_side);
    int mobility_heuristic(Board* board, Side move_side);
    
    int get_weight(Board* board, Side move_side, int i, int j);

    int heuristic(Board* board, Side move_side);

    // Minimax
    double minimax(Board* board, int depth, bool maximizing, double& a, double& b);

    // Game code
    Move *doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
};
