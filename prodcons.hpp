/*
 *  Producer-buffer-consumer with threads
 */

#ifndef PRODCONS_HPP
#define PRODCONS_HPP

#include "tsqueue.hpp"

#include <optional>
#include <thread>


template<typename T> class prod_cons {
public:
    //prod_cons(buffer<T>& buf, function< optional<T>() > prod, function<void(const T&)> cons):
    prod_cons(buffer<T>& buf, const auto& prod, const auto& cons, bool autorun=true):
        buf(buf), prod(prod), cons(cons) {
        if (autorun)
            run();
    }

    // produce loop
    void produce() {
        for(;;) {
            optional<T> e = prod(); // optnull is signal of End-Of-Data
            if (e.has_value()) {
                buf.put(e.value());
            } else {
                buf.set_end();
                return;
            }
        }
    }

    // consume loop
    void consume() {
        T e;
        while (!buf.end()) {
            buf.get(e);
            cons(e);
        }
    }

    void run() {
        // start the produce and consume loops
        thread thread_producer(&prod_cons<T>::produce, this);
        thread thread_consumer(&prod_cons<T>::consume, this);

        // wait for the ends of both thread
        thread_producer.join();
        thread_consumer.join();
    }


protected:
    buffer<T>& buf;
    const function< optional<T>() >& prod;
    const function<void(const T&) >& cons;

};


#endif // PRODCONS_HPP
