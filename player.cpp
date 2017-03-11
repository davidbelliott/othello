#include "player.hpp"
#include <map>
#include <limits>
#include <vector>

const int Player::weights[8][8] = 
   {{  4, -3,  2,  2,  2,  2, -3,  4 },
    { -3, -4, -1, -1, -1, -1, -4, -3 },
    {  2, -1,  1,  0,  0,  1, -1,  2 },
    {  2, -1,  0,  1,  1,  0, -1,  2 },
    {  2, -1,  0,  1,  1,  0, -1,  2 },
    {  2, -1,  1,  0,  0,  1, -1,  2 },
    { -3, -4, -1, -1, -1, -1, -4, -3 },
    {  4, -3,  2,  2,  2,  2, -3,  4 }};
/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side side_in)
    : board(new Board()),
      side(side_in),
      testingMinimax(false)   // Will be set to true in test_minimax.cpp.
{
}

/*
 * Destructor for the player.
 */
Player::~Player()
{
    delete board;
}

void Player::set_board(Board* board_in)
{
    board = board_in;
}

double Player::minimax(Board* board, int depth, bool maximizing, double& a, double& b)
{
    double score = 0.0;
    if(depth == 0)  // Reached bottom
        score = heuristic(board, side);
    else
    {
        // Score starts out at -Inf for max, Inf for min
        score = maximizing ? -std::numeric_limits<double>::max() : std::numeric_limits<double>::max();
        Move move(0, 0);
        bool prune = false;
        for(int i = 0; i < 8 && !prune; i++)
        {
            for(int j = 0; j < 8 && !prune; j++)
            {
                move.set_x(i);
                move.set_y(j);
                Side moving_side = maximizing ? side : OTHER_SIDE(side);
                if(board->checkMove(&move, moving_side))
                {
                    // If there's an available move, min/max resulting state
                    Board* copy = board->copy();
                    copy->doMove(&move, moving_side);
                    double this_score = minimax(copy, depth - 1, !maximizing, a, b);
                    delete copy;
                    if(maximizing)
                    {
                        score = std::max(score, this_score);
                        a = std::max(a, score);
                        if(b <= a)  // Prune this subtree
                            prune = true;
                    }
                    else
                    {
                        score = std::min(score, this_score);
                        b = std::min(b, score);
                        if(b <= a)  // Prune this subtree
                            prune = true;
                    }
                }
            }
        }
    }
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
    return n_corners + n_potential;
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
double Player::heuristic(Board* board, Side move_side)
{
    if(testingMinimax)
        return parity_heuristic(board, move_side);
    else
    {
        /*std::cerr << "Corner heuristic: " << corner_heuristic(board, move_side)
                  << "\nMobility heuristic: " << mobility_heuristic(board, move_side)
                  << "\nParity heuristic: " << parity_heuristic(board, move_side)
                  << std::endl;*/
        Side min_side = OTHER_SIDE(move_side);
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
        
        return corner + stability + parity + mobility;/*mobility + stability + parity*/;
    }
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
    board->doMove(opponentsMove, OTHER_SIDE(side));

    bool found_move = false;
    std::pair<double, Move> best_move(0.0, Move(0, 0));
    Move move(0, 0);
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            move.set_x(i);
            move.set_y(j);
            if(board->checkMove(&move, side))
            {
                double a = -std::numeric_limits<double>::max();
                double b = -a;
                Board* copy = board->copy();
                copy->doMove(&move, side);
                //double score = minimax(copy, testingMinimax ? 1 : 4, true, a, b);
                double score = heuristic(copy, side);
                delete copy;
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
        board->doMove(&best_move.second, side);
        ret = new Move(best_move.second);
    }

    return ret;
}
