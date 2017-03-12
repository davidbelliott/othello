#include "player.hpp"
#include <map>
#include <limits>
#include <vector>
#include <chrono>
#include <thread>

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
Player::Player(Side player_side_in)
    : board(new Board()),
      player_side(player_side_in),
      last_move_time(),
      testingMinimax(false)   // Will be set to true in test_minimax.cpp.
{
    last_move_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    //std::this_thread::sleep_for(std::chrono::seconds(5));
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
    delete board;
    board = board_in;
}

void Player::get_possible_moves(Board* board, Side side, std::vector<Move>* moves)
{
    *moves = { };
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            Move move(i, j);
            if(board->checkMove(&move, side))
                moves->push_back(move);
        }
    }
}

int Player::negamax(Board* board, int depth, Side move_side, Move** m)
{
    if(m)
        *m = nullptr;

    if(depth == 0)
        return heuristic(board, move_side);

    std::vector<Move> moves;
    get_possible_moves(board, move_side, &moves);

    if(moves.empty())
        return -35;

    Move best_move(0, 0);
    int best_score = -std::numeric_limits<int>::max();

    for(auto it = moves.begin(); it != moves.end(); ++it)
    {
        Board copy = *board;
        copy.doMove(&*it, move_side);
        int this_score = (int)((float)heuristic(board, move_side) - 0.85 * (double)negamax(&copy, depth - 1, OTHER_SIDE(move_side)));
        if(this_score > best_score)
        {
            best_score = this_score;
            best_move = *it;
        }
        /*else if(!maximizing && this_score < best_score)
        {
            best_score = this_score;
            best_move = *it;
        }*/
    }

    if(m)
        *m = new Move(best_move);

    if(depth == 4)
        int x = 0;

    return best_score;
}

/**
 * @brief Returns the difference between move_side's pieces and the other
 * side's pieces.
 */
/*int Player::parity_heuristic(Board* board, Side move_side)
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
}*/

/**
 * @brief Returns a weighted sum of all heuristics.
 */
int Player::heuristic(Board* board, Side move_side)
{
    Side other_side = OTHER_SIDE(move_side);
    /*if(testingMinimax)
        return board->count(move_side) - board->count(other_side);
    else
    {*/
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
    //}
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
    board->doMove(opponentsMove, OTHER_SIDE(player_side));
    Move* best_move = nullptr;
    negamax(board, testingMinimax ? 2 : 5, player_side, &best_move);
    if(best_move)
    {
        board->doMove(best_move, player_side);
        std::cerr << "Did (" << best_move->x << ", " << best_move->y << ")\n";
    }
    return best_move;
}
