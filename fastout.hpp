/*
 *  Transparent ostream writer. To avoid extra message fragmentation.
 */

#ifndef FASTOUT_HPP
#define FASTOUT_HPP

#include <sstream>

// this wrapper practically avoids mixing of error messages with multithreading
class fastout : public std::stringstream {
public:
    fastout(std::ostream& out) : out(out) {}
    ~fastout() { *this << std::endl; out << str(); }

protected:
    std::ostream &out;
};

#endif // FASTOUT_HPP
