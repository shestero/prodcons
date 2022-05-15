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
    //typedef optional<T> opT;
    using opT=optional<T>;

    prod_cons(safe_buffer<opT>& buf, const function<opT()>& prod, const function<void(const T&)>& cons, bool autorun=true):
        buf(buf), prod(prod), cons(cons)
    {
        if (autorun)
            run();
    }

    // produce loop
    void produce() {
        opT op;
        for(;;) {
            op = prod(); // optnull is signal of End-Of-Data
            buf.put(op);
            if (!op.has_value()) {
                // end of data; exit the producer loop
                return;
            }
        }
    }

    // consume loop
    void consume() {
        opT op;
        for(;;) {
            buf.get(op);
            if (!op.has_value()) {
                // end of data; exit the consumer loop
                return;
            }
            cons(op.value());
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
    safe_buffer<opT>& buf;
    const function<opT()> prod;
    const function<void(const T&)> cons;

};


#endif // PRODCONS_HPP
