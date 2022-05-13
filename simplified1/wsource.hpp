/*
 *  Helper to get word by word stream source from command line (pull way)
 */

#ifndef WSOURCE_HPP
#define WSOURCE_HPP

#include <optional>

// function object - source of words from command line (to pull)
class words_source {
public:
    words_source(int argc, char *argv[]) : cnt(argc), word(argv) {}

    std::optional<char *> operator()() {
        if(cnt<=1)
            return std::nullopt;

        --cnt;
        return std::optional<char *>(*++word);
    }

protected:
    int cnt;     // counting down; when 1 there is no more words
    char** word; // points the last word, to be moved up
};


#endif // WSOURCE_HPP
