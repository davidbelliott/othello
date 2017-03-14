#include "player.hpp"
#include "opening_book.hpp"
#include <map>
#include <limits>
#include <vector>
#include <thread>
#include <time.h>

const int Player::weights[8][8] = 
   {{  5, -3,  2,  2,  2,  2, -3,  5 },
    { -3, -5, -1, -1, -1, -1, -5, -3 },
    {  2, -1,  1,  0,  0,  1, -1,  2 },
    {  2, -1,  0,  1,  1,  0, -1,  2 },
    {  2, -1,  0,  1,  1,  0, -1,  2 },
    {  2, -1,  1,  0,  0,  1, -1,  2 },
    { -3, -5, -1, -1, -1, -1, -5, -3 },
    {  5, -3,  2,  2,  2,  2, -3,  5 }};

const int Player::max_depth = 100;
const int Player::default_depth = 5;
/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(char player_side_in)
    : board(new Board()),
      erm(30),
      player_side(player_side_in),
      testingMinimax(false)   // Will be set to true in test_minimax.cpp.
{
    /*char test[] = "abcdefgh"
                  "ijklmnop"
                  "qrstuvwx"
                  "yz012345"
                  "01234567"
                  "01234567"
                  "01234567"
                  "01234567";

    Move move(4, 6);

    for(int j = 0; j < 64; j++)
    {
        if(j % 8 == 0)
            std::cerr << "\n";
        std::cerr << test[j];
    }
    std::cerr << "\tMove:" << move.x << ", " << move.y << std::endl;
    char rotated[64];
    for(int i = 0; i < 4; i++)
    {
        rotate_data(test, rotated);
        move = rotate_move(move, 1);
        memcpy(test, rotated, 64);
        for(int j = 0; j < 64; j++)
        {
            if(j % 8 == 0)
                std::cerr << "\n";
            std::cerr << test[j];
        }
        std::cerr << "\tMove:" << move.x << ", " << move.y << std::endl;
    }*/
    //std::this_thread::sleep_for(std::chrono::seconds(10));

    //load_book("presbyterian_ghostbusters_moves");
}

/*
 * Destructor for the player.
 */
Player::~Player()
{
    delete board;
}

/*
 * @brief Sets the board to board_in.
 */
void Player::set_board(Board* board_in)
{
    delete board;
    board = board_in;
}

void Player::get_possible_moves(Board* board, char side, std::vector<Move>* moves)
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

int Player::negamax(Board* board, int depth, char move_side, int a, int b, Move** m)
{
    int best_score = -INFINITY;
    // Check if exists in transposition table

    // If passing move, start out with nullptr
    if(m)
        *m = nullptr;

    // If reached bottom, return heuristic of this state
    if(depth == 0)
        return heuristic(board, move_side);

    char other_side = OTHER_SIDE(move_side);
    std::vector<Move> moves;
    get_possible_moves(board, move_side, &moves);

    // If no moves available for this side
    if(moves.empty())
    {
        std::vector<Move> other_moves;
        get_possible_moves(board, other_side, &other_moves);
        if(other_moves.empty())   // Game endpoint
        {
            int diff = board->count(move_side) - board->count(other_side);
            if(diff > 0)
                best_score = INFINITY / 2;    // Win is infinitely valuable, but
                                        // must divide by 2 for maximizing
            else if(diff < 0)
                best_score = -INFINITY / 2;   // Loss is infinitely bad, but
                                        // must divide by 2 for maximizing
            else
                best_score = 0;           // Draw is neutral
        }
        else
            best_score = -20;             // We don't want to pass a move
    }
    else
    {
        Move best_move(0, 0);
        for(auto it = moves.begin(); it != moves.end(); ++it)
        {
            Board copy = *board;
            copy.doMove(&*it, move_side);
            int this_score = -negamax(&copy, depth - 1, other_side, -b, -a);
            if(this_score > best_score)
            {
                best_score = this_score;
                best_move = *it;
                a = this_score;
                if(a >= b)  // Prune branch
                    break;
            }
        }

        if(m)
            *m = new Move(best_move);
    }
    return best_score;
}

/**
 * @brief Returns a weighted sum of all heuristics.
 */
int Player::heuristic(Board* board, char move_side)
{
    char other_side = OTHER_SIDE(move_side);
    if(testingMinimax)
        return board->count(move_side) - board->count(other_side);
    else
    {
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
    }
}

int Player::get_weight(Board* board, char othelloside, int i, int j)
{
    return weights[i][j];
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
    clock_t begin_time = clock();
    board->doMove(opponentsMove, OTHER_SIDE(player_side));
    Move* best_move = nullptr;
    bool found_opening_book_move = false;

    //print_board(board->data, std::cerr);

    if(testingMinimax)
        negamax(board, 2, player_side, -INFINITY, INFINITY, &best_move);
    else 
    {
        char rotated[65];
        char temp[65];

        rotated[64] = '\0';
        temp[64] = '\0';
        memcpy(rotated, board->data, 64);
        for(int i = 0; i < 4; i++)
        {
            //std::cerr << "Lookup str: " << rotated << std::endl;
            /*for(auto it = opening_book.begin(); it != opening_book.end(); ++it)
                std::cerr << "Comparison str: " << it->first << std::endl;*/
            if(opening_book.count(rotated))
            {
                //std::cerr << "Located opening book move" << std::endl;
                Move rotated_move = rotate_move(opening_book[rotated], i);
                if(board->checkMove(&rotated_move, player_side))
                {
                    best_move = new Move(rotated_move);
                    found_opening_book_move = true;
                    break;
                }
            }
            rotate_data(rotated, temp);
            strcpy(rotated, temp);
        }

        if(found_opening_book_move)
            std::cerr << "Used opening book to get move: " << best_move->x << ", " << best_move->y << std::endl;
        else
        {
            clock_t next_expected_ms = 0;
            int depth = 1;
            if(msLeft == -1)
            {
                depth = default_depth;
                negamax(board, depth, player_side, -INFINITY, INFINITY, &best_move);
            }
            else
            {
                for(depth = 1; depth <= max_depth && ((clock() - begin_time + next_expected_ms) * 1000) / CLOCKS_PER_SEC < msLeft / erm; depth++)
                {
                    clock_t iter_start_time = clock();
                    negamax(board, depth, player_side, -INFINITY, INFINITY, &best_move);
                    next_expected_ms = (clock() - iter_start_time) * 4;
                }
            }
            std::cerr << "Ran to depth " << depth << " in " << (((clock() - begin_time) * 1000) / CLOCKS_PER_SEC) << " ms\n";
        }
    }

                
    if(best_move)
        board->doMove(best_move, player_side);
    if(erm > 1)
        erm--;

    /*if(best_move && !found_opening_book_move)
    {
        std::cerr << "Adding move:\n";
        print_board(board->data, std::cerr);
        std::cerr << "\nMove:" << best_move->x << ", " << best_move->y << std::endl;
        opening_book[board->data] = *best_move;
    }*/
    /*if(board->isDone())
        write_book("presbyterian_ghostbusters_moves");*/
    return best_move;
}
