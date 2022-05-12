/*
 *  Collect data to fixed-size portions
 */

#ifndef CHUNKER_HPP
#define CHUNKER_HPP

#include <algorithm>
#include <functional>

// forwards the data by fixed-size portions
template<typename T,unsigned int chunkSize> class chunker {
public:
    chunker(const std::function<void(const T*) > &receiver): receiver(receiver), pos(0) {
    }

    void operator()(const T& e) {
        chunk[pos++] = e;
        if (pos>=chunkSize) {
            receiver(chunk);
            pos=0;
        }
    }

    bool empty() const { return pos==0; }
    size_t count() const { return pos; }

protected:
    const std::function<void(const T*) > &receiver;

    size_t pos;
    T chunk[chunkSize];
};

#endif // CHUNKER_HPP
