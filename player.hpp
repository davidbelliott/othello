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
    Board* board;
    Side player_side;
    static const int weights[8][8];
    milliseconds last_move_time;


public:
    
    // Constructors / Destructors
    Player(Side side_in);
    ~Player();

    // Heuristics
    /*int parity_heuristic(Board* board, Side move_side);
    int corner_heuristic(Board* board, Side move_side);
    int stability_heuristic(Board* board, Side move_side);
    int mobility_heuristic(Board* board, Side move_side);*/

    void set_board(Board* board);
    
    int get_weight(Board* board, Side move_side, int i, int j);

    int heuristic(Board* board, Side move_side);

    void get_possible_moves(Board* board, Side side, std::vector<Move>* moves);

    // Negamax
    int negamax(Board* board, int depth, Side move_side, int a, int b, Move** m=nullptr);

    // Game code
    Move *doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
};
