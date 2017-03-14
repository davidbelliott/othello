#include "board.hpp"

/*
 * Make a standard 8x8 othello board and initialize it to the standard setup.
 */
Board::Board()
    : data()
{
    for(int i = 0; i < 64; i++)
        data[i] = ' ';
    set(WHITE, 3, 3);
    set(WHITE, 4, 4);
    set(BLACK, 3, 4);
    set(BLACK, 4, 3);
}

Board::Board(char data_in[])
    : Board()
{
    for(int i = 0; i < 64; i++)
        data[i] = data_in[i];
}

/*
 * Destructor for the board.
 */
Board::~Board() {
}

bool Board::onBoard(int x, int y)
{
    return 0 <= x && x <= 7 && 0 <= y && y <= 7;
}

bool Board::get(char side, int x, int y)
{
    return data[x+8*y] == side;
}

void Board::set(char side, int x, int y)
{
    data[x+8*y] = side;
}

bool Board::occupied(int x, int y)
{
    return data[x+8*y] != ' ';
}

/*
 * Returns true if the game is finished; false otherwise. The game is finished
 * if neither side has a legal move.
 */
bool Board::isDone() {
    return !(hasMoves(BLACK) || hasMoves(WHITE));
}

/*
 * Returns true if there are legal moves for the given side.
 */
bool Board::hasMoves(char side)
{
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Move move(i, j);
            if (checkMove(&move, side)) return true;
        }
    }
    return false;
}

/*
 * Returns true if a move is legal for the given side; false otherwise.
 */
bool Board::checkMove(Move *m, char side)
{
    // Passing is only legal if you have no moves.
    if (m == nullptr) return !hasMoves(side);

    int X = m->get_x();
    int Y = m->get_y();

    // Make sure the square hasn't already been taken.
    if (occupied(X, Y)) return false;

    char other = OTHER_SIDE(side);
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            // Is there a capture in that direction?
            int x = X + dx;
            int y = Y + dy;
            if (0 <= x && x <= 7 && 0 <= y && y <= 7 && get(other, x, y)) {
                do {
                    x += dx;
                    y += dy;
                } while (0 <= x && x <= 7 && 0 <= y && y <= 7 && get(other, x, y));

                if (0 <= x && x <= 7 && 0 <= y && y <= 7 && get(side, x, y)) return true;
            }
        }
    }
    return false;
}

/*
 * Modifies the board to reflect the specified move.
 */
void Board::doMove(Move *m, char side) {
    // A nullptr move means pass.
    if (m == nullptr) return;

    // Ignore if move is invalid.
    if (!checkMove(m, side)) return;

    int X = m->get_x();
    int Y = m->get_y();
    char other = OTHER_SIDE(side);

    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            int x = X;
            int y = Y;
            do {
                x += dx;
                y += dy;
            } while (0 <= x && x <= 7 && 0 <= y && y <= 7 && get(other, x, y));

            if (onBoard(x, y) && get(side, x, y)) {
                x = X;
                y = Y;
                x += dx;
                y += dy;
                while (onBoard(x, y) && get(other, x, y)) {
                    set(side, x, y);
                    x += dx;
                    y += dy;
                }
            }
        }
    }
    set(side, X, Y);
}

/*
 * Current count of given side's stones.
 */
int Board::count(char side)
{
    int total = 0;
    for(int i = 0; i < 64; i++)
        total += data[i] == side;
    return total;
}

void Board::setBoard(char data[])
{
    for(int i = 0; i < 64; i++)
        this->data[i] = data[i];
}

bool Board::operator<(Board& other)
{
    return this->data < other.data;
}
