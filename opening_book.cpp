#include "opening_book.hpp"
#include <fstream>
#include <iostream>

void print_board(const char board[64], std::ostream& out)
{
    for(int j = 0; j < 64; j++)
    {
        out << board[j];
        if(j % 8 == 7)
            out << "\n";
    }
}

std::map<std::string, Move> opening_book = {
    // Diagonal opening
    {
        "        "
        "        "
        "        "
        "   wb   "
        "   bw   "
        "        "
        "        "
        "        ",
        {2, 3}
    },
    {
        "        "
        "        "
        "        "
        "  bbb   "
        "   bw   "
        "        "
        "        "
        "        ",
        {2, 2}
    },

    // X-square opening
    {
        "        "
        "        "
        "  wb    "
        "  bbb   "
        "   bw   "
        "        "
        "        "
        "        ",
        {2, 4}
    },
    {
        "        "
        " b      "
        "  bb    "
        "  wbb   "
        "  www   "
        "        "
        "        "
        "        ",
        {0, 0}
    },

    // Snake/Peasant
    {
        "        "
        "        "
        " bbb    "
        "  wbb   "
        "  www   "
        "        "
        "        "
        "        ",
        {5, 3}
    },

    // Lysons
    {
        "        "
        "        "
        " bbb w  "
        "  wbw   "
        "  www   "
        "        "
        "        "
        "        ",
        {2, 5}
    },

    // Pyramid/Checkerboarding Peasant
    {
        "        "
        "        "
        " bbb    "
        " wbwbw  "
        " bbbbb  "
        "  bw    "
        "        "
        "        ",
        {1, 5}
    },

    // Heath/Tobidashi "Jumping Out"
    {
        "        "
        "        "
        "  wb    "
        " bbbb   "
        "  www   "
        "        "
        "        "
        "        ",
        {3, 1}
    },

    // Classic Heath
    {
        "        "
        "  bw    "
        "  bb    "
        " bbwb   "
        "  www   "
        "        "
        "        "
        "        ",
        {0, 3}
    },

    // Mimura Variation II
    {
        "        "
        "  bw    "
        "  bb    "
        " bbbbw  "
        "  bbbb  "
        "  wwb   "
        "     b  "
        "        ",
        {5, 5}
    },

    // Heath-Bat
    {
        "        "
        "   w    "
        "  ww    "
        " bbwb   "
        "  bww   "
        "   b    "
        "        "
        "        ",
        {1, 2}
    },

    // Iwasaki Varation
    {
        "        "
        "   wb   "
        "  wb    "
        " bbwb   "
        "  www   "
        "        "
        "        "
        "        ",
        {1, 2}
    }

    // Heath-Chimney 'Mass-Turning'
};

void load_book(const char* filename)
{
    std::ifstream input_file(filename);
    while(input_file.good() && !input_file.eof())
    {
        char board[65];
        board[64] = '\0';
        for(int i = 0; i < 8; i++)
        {
            for(int j = 0; j < 8; j++)
                board[8*i + j] = input_file.get();
            input_file.get();
        }
        if(input_file.good() && !input_file.eof())
        {
            int x, y;
            input_file >> x;
            input_file.get();
            input_file >> y;
            input_file.get();
            Move move(x, y);
            std::cerr << "Assigning move (" << x << ", " << y <<") to board:\n";
            print_board(board, std::cerr);
            std::cerr << std::endl;
            opening_book[board] = move;
        }
    }
    input_file.close();
}

void write_book(const char* filename)
{
    std::ofstream output_file(filename);
    for(auto it = opening_book.begin(); it != opening_book.end(); ++it)
    {
        print_board(it->first.c_str(), output_file);
        output_file << it->second.x << "," << it->second.y << "\n";
    }
    output_file.close();
}

