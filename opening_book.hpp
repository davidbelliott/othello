#pragma once

#include "common.hpp"
#include <map>
//#include <string.h>

/*struct CstrCmp
{
    bool operator()(const char* a, const char* b) const
    {
        return (memcmp(a, b, 64) < 0);
    }
};*/

extern std::map<std::string, Move> opening_book;

void print_board(const char board[64], std::ostream& out);
void load_book(const char* filename);
void write_book(const char* filename);
