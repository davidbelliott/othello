#include "opening_book.hpp"

std::map<const char*, Move, CstrCmp> opening_book = {
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
        "        "
        "        "
        "        "
        "        "
        "        "
        "        "
        "        ",
        {0, 0}
    },
    {
        "        "
        "        "
        "        "
        "        "
        "        "
        "        "
        "        "
        "        ",
        {0, 0}
    },
    {
        "        "
        "        "
        "        "
        "        "
        "        "
        "        "
        "        "
        "        ",
        {0, 0}
    },
    {
        "        "
        "        "
        "        "
        "        "
        "        "
        "        "
        "        "
        "        ",
        {0, 0}
    }
};
