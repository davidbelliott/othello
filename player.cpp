#include "player.hpp"
#include <map>
#include <limits>
#include <vector>

const int Player::weights[8][8] = 
   {{  5, -3,  2,  2,  2,  2, -3,  5 },
    { -3, -5, -1, -1, -1, -1, -5, -3 },
    {  2, -1,  1,  0,  0,  1, -1,  2 },
    {  2, -1,  0,  1,  1,  0, -1,  2 },
    {  2, -1,  0,  1,  1,  0, -1,  2 },
    {  2, -1,  1,  0,  0,  1, -1,  2 },
    { -3, -5, -1, -1, -1, -1, -5, -3 },
    {  5, -3,  2,  2,  2,  2, -3,  5 }};
/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side side_in)
    : board(),
      side(side_in),
      last_move_time(),
      testingMinimax(false)   // Will be set to true in test_minimax.cpp.
{
    last_move_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
}

/*
 * Destructor for the player.
 */
Player::~Player()
{
}

double Player::minimax(Board* board, int depth, bool maximizing, double& a, double& b)
{
    Side moving_side = maximizing ? side : OTHER_SIDE(side);
    Side other_side = OTHER_SIDE(moving_side);
    double score = 0;
    bool this_can_move = false;
    bool other_can_move = false;
    if(depth == 0)  // Reached bottom
        score = heuristic(board, moving_side);
    else
    {
        // Score starts out at -Inf if max, Inf if min
        score = -std::numeric_limits<double>::max();
        Move move(0, 0);
        bool prune = false;
        for(int i = 0; i < 8 && !prune; i++)
        {
            for(int j = 0; j < 8 && !prune; j++)
            {
                move.set_x(i);
                move.set_y(j);
                if(board->checkMove(&move, moving_side))
                {
                    // If there's an available move, min/max resulting state
                    this_can_move = true;
                    Board copy = *board;
                    copy.doMove(&move, moving_side);
                    double this_score = 2 * heuristic(&copy, moving_side) - 0.8 * minimax(&copy, depth - 1, !maximizing, a, b);
                    //if(maximizing)
                    {
                        score = std::max(score, this_score);
                        a = std::max(a, score);
                    }
                    /*else
                    {
                        score = std::min(score, this_score);
                        b = std::min(b, score);
                    }
                    if(b <= a)  // Prune this subtree
                    {
                        std::cerr << "Pruning" << std::endl;
                        prune = true;
                    }*/
                }
                if(board->checkMove(&move, other_side))
                    other_can_move = true;
            }
        }
    }

    if(!this_can_move)
        score = -50.0;

    return score;
}

/**
 * @brief Returns the difference between move_side's pieces and the other
 * side's pieces.
 */
int Player::parity_heuristic(Board* board, Side move_side)
{
    return board->count(move_side);
}

int Player::mobility_heuristic(Board* board, Side move_side)
{
    int n_moves = 0;
    Move move(0, 0);
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            move.set_x(i);
            move.set_y(j);
            if(board->checkMove(&move, move_side))
                n_moves++;
        }
    }
    return n_moves;
}

int Player::corner_heuristic(Board* board, Side move_side)
{
    int n_corners = 0;
    int n_potential = 0;
    Move move(0, 0);
    for(int i = 0; i < 8; i += 7)
    {
        for(int j = 0; j < 8; j += 7)
        {
            if(board->get(move_side, i, j))
                n_corners++;
            else
            {
                move.set_x(i);
                move.set_y(j);
                if(board->checkMove(&move, move_side))
                    n_potential++;
            }
        }
    }
    return n_corners;
}

int Player::stability_heuristic(Board* board, Side move_side)
{
    int stability = 0;
    Side other = OTHER_SIDE(move_side);
    Move move(0, 0);
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            if(board->get(move_side, i, j))
            {
                bool adjacent_empty = false;
                bool can_flank = false;
                for(int k = -1; k <= 1 && !can_flank; k++)
                {
                    for(int l = -1; l <= 1 && !can_flank; l++)
                    {
                        if(k == 0 && l == 0)
                            continue;
                        if(!board->occupied(i + k, j + l) && !board->occupied(i - k, j - l))
                        {
                            adjacent_empty = true;
                            if(board->get(other, i - k, j - l))
                                can_flank = true;
                        }
                    }
                }
                if(can_flank)
                    stability--;
                else if(!adjacent_empty)
                    stability++;
            }
        }
    }
                    
    return stability;
}

/**
 * @brief Returns a weighted sum of all heuristics.
 */
int Player::heuristic(Board* board, Side move_side)
{
    Side other_side = OTHER_SIDE(move_side);
    if(testingMinimax)
        return parity_heuristic(board, move_side) - parity_heuristic(board, other_side);
    else
    {
        /*std::cerr << "Corner heuristic: " << corner_heuristic(board, move_side)
                  << "\nMobility heuristic: " << mobility_heuristic(board, move_side)
                  << "\nParity heuristic: " << parity_heuristic(board, move_side)
                  << std::endl;*/
        /*Side min_side = OTHER_SIDE(move_side);
        int max_corner_val = corner_heuristic(board, move_side);
        int min_corner_val = corner_heuristic(board, min_side);
        double corner_weight = 30.0;
        double corner = (max_corner_val + min_corner_val == 0 ? 0.0 :
                corner_weight * (max_corner_val - min_corner_val) /
                (max_corner_val + min_corner_val));
        
        int max_mobility_val = mobility_heuristic(board, move_side);
        int min_mobility_val = mobility_heuristic(board, min_side);
        double mobility_weight = 5.0;
        double mobility = (max_mobility_val + min_mobility_val == 0 ? 0.0 :
                mobility_weight * (max_mobility_val - min_mobility_val) / 
                (max_mobility_val + min_mobility_val));

        int max_stability_val = stability_heuristic(board, move_side);
        int min_stability_val = stability_heuristic(board, min_side);
        double stability_weight = 25.0;
        double stability = (max_stability_val + min_stability_val == 0 ? 0.0 :
                stability_weight * (max_stability_val - min_stability_val) / 
                (max_stability_val + min_stability_val));

        int max_parity_val = parity_heuristic(board, move_side);
        int min_parity_val = parity_heuristic(board, min_side);
        double parity_weight = 25.0;
        double parity = (max_parity_val + min_parity_val == 0 ? 0.0 :
                parity_weight * (max_parity_val - min_parity_val) /
                (max_parity_val + min_parity_val));
                */

        int total = 0;
        for(int i = 0; i <= 7; i++)
        {
            for(int j = 0; j <= 7; j++)
            {
                if(board->get(move_side, i, j))
                    total += get_weight(board, move_side, i, j);
                else if(board->get(other_side, i, j))
                    total -= get_weight(board, other_side, i, j);
            }
        }
        return total;

        /*int total = 0;
        for(int i = 0; i < 8; i++)
        {
            for(int j = 0; j < 8; j++)
            {
                total += (board->get(move_side, i, j) ? weights[i][j] : 0);
                total -= (board->get(other_side, i, j) ? weights[i][j] : 0);
            }
        }*/

        //return total;
    }
}

int Player::get_weight(Board* board, Side othelloside, int i, int j)
{
    //return weights[i][j];
    int k = 1;
    if((i == 0 || i == 7) && (j == 0 || j == 7))
        k += 40;
    if(i == 0 || i == 7 || j == 0 || j == 7)
        k += 10;
    if(i == 1 || i == 6 || j == 1 || j == 6)
    {
        k--;
        if(i == 1 && j == 1 && !board->get(othelloside, 0, 0))
            k -= 5;
        if(i == 1 && j == 6 && !board->get(othelloside, 0, 7))
            k -= 5;
        if(i == 6 && j == 1 && !board->get(othelloside, 7, 0))
            k -= 5;
        if(i == 6 && j == 6 && !board->get(othelloside, 7, 7))
            k -= 5;
    }

    return k;
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be
 * nullptr.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return nullptr.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft)
{
    board.doMove(opponentsMove, OTHER_SIDE(side));

    milliseconds elapsed = duration_cast<milliseconds>(system_clock::now().time_since_epoch()) - last_move_time;
    bool found_move = false;
    std::pair<int, Move> best_move(0.0, Move(0, 0));
    Move move(0, 0);
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            move.set_x(i);
            move.set_y(j);
            if(board.checkMove(&move, side))
            {
                double a = -std::numeric_limits<double>::max();
                double b = -a;
                Board copy = board;
                copy.doMove(&move, side);
                double score = heuristic(&copy, side) - 0.8 * minimax(&copy, testingMinimax ? 1 : 3, false, a, b);
                if(!found_move || score > best_move.first)
                {
                    found_move = true;
                    best_move.first = score;
                    best_move.second = move;
                }
            }
        }
    }

    Move* ret = nullptr;
    if(found_move)
    {
        board.doMove(&best_move.second, side);
        ret = new Move(best_move.second);
    }

    last_move_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    return ret;
}
