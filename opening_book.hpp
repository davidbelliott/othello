#pragma once

#include "common.hpp"
#include <map>
#include <string.h>

struct CstrCmp
{
    bool operator()(const char* a, const char* b) const
    {
        return memcmp(a, b, 64) < 0;
    }
};

extern std::map<const char*, Move, CstrCmp> opening_book;
