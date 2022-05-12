/*
 *  String-to-Long converter
 */

#ifndef FATOL_HPP
#define FATOL_HPP

#include <limits.h>
#include <optional>

#include "fastout.hpp"


// Fast string to long integer parser.
// It mooved the pointer p.
// if p was mooved to the end of line (*p)=0 the parsing was ok.
// if *p not 0 after the end of the function there is a problem and *p points into the problem place.
// if *p is point to digital char after the end of the function then it is overflow (for long)
// Note: empty input line gives result 0, ok.
long fast_atol(char* &p)
{
    unsigned long long u = 0;
    const bool neg = *p=='-'; // negative number
    if (neg) ++p;
    for(; *p>='0' && *p <='9'; ++p) {
        u = (u << 1) + (u << 3) + *p - '0';
        if (u>LONG_MAX) { // overflow
            return 0;
        }
    }
    return neg? -u : u ;
}

// fast_atol wrapper
std::optional<long> word2long(char* w) {
    long l = fast_atol(w);
    if (*w==0) { // ok
        return std::optional<long>(l);
    } else {
        fastout(std::cerr) << "ERROR\tCannot convert the word to number. Fail at '" << w << "'";
        return std::nullopt;
    }
}

#endif // FATOL_HPP
