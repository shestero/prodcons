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
    chunker(const std::function<void(const T*) > &receiver): receiver(receiver) {}
    ~chunker() {
        // Warning about unused data
        if (pos>0) {
            fastout(std::cerr) << "WARNING\tLast " << pos
                               << " object(s) of unused data is/are ignored!";
        }

    }

    void operator()(const T& e) {
        chunk[pos++] = e;
        if (pos>=chunkSize) {
            receiver(chunk);
            pos=0;
        }
    }

protected:
    const std::function<void(const T*)> receiver;

    size_t pos{0};
    T chunk[chunkSize];
};

#endif // CHUNKER_HPP
