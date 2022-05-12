/*
 *  A set of FIFO thread-safe queue buffers implementations.
 *  buffer                  - a common interface
 *  queue_thread_safe       - a basic thread safe queue without bound control
 *  queue_safe_and_limited  - the same with limited capacity and overdry control blocking
 *  queue_with_end          - the same with End-Of-Data support (used nesting into optional<>)
 */

#ifndef TSQUEUE_HPP
#define TSQUEUE_HPP

#include <cassert>
#include <queue>
#include <optional>
#include <functional>
#include <mutex>
#include <semaphore>


// Interface for FIFO buffer - queue of T
template<typename T> class buffer {
public:
    virtual void put(const T&) = 0;
    virtual void get(T& e) = 0;

    virtual void set_end() { assert(false); }  // not defined
    virtual bool end() { return false; }       // endless
};

// Implementations:

using namespace std;

// Thread-safe FIFO buffer-queue
template<typename T> class queue_thread_safe : public buffer<T>, protected queue<T> {
public:
    virtual void put(const T& e) {
        lock_guard<mutex> lock(m);
        queue<T>::push(e);
    }
    // Warning: this get() don't chek bounds; it produces run-time error if is called on empty queue!
    virtual void get(T& e) {
        lock_guard<mutex> lock(m);
        e = queue<T>::front();
        queue<T>::pop();
    }

protected:
    mutex m;
};

// Thread-safe FIFO buffer-queue with limited size (to block producer).
// It can also block the consumer if it reads the dryned queue.
template<typename T,unsigned int maxSize> class queue_safe_and_limited : public queue_thread_safe<T> {
public:
    virtual void put(const T& e) {
        number_of_empty_positions.acquire();
        queue_thread_safe<T>::put(e);
        number_of_queueing_portions.release();
    }
    virtual void get(T& e) {
        number_of_queueing_portions.acquire();
        queue_thread_safe<T>::get(e);
        number_of_empty_positions.release();
    }

protected:
    counting_semaphore<maxSize> number_of_queueing_portions{0};         // to block consumption
    counting_semaphore<maxSize> number_of_empty_positions{maxSize};     // to block production
};

// Queue that support end-of-stream feature ( set_end() and bool end() )
template <typename T,unsigned int maxSize> class queue_with_end :
         public buffer<T>, protected queue_safe_and_limited<optional<T>,maxSize> {
public:
     virtual void put(const T& e) {
         queue_safe_and_limited<optional<T>,maxSize>::put(optional(e));
     }
     virtual void get(T& e) {
         optional<T> op;
         queue_safe_and_limited<optional<T>,maxSize>::get(op);
         e = op.value(); // do runtime error if it is END
     }

     virtual void set_end() {
         queue_safe_and_limited<optional<T>,maxSize>::put(nullopt); // (optional<T>());
     }
     virtual bool end() {
         // ensure the buffer isn't full
         queue_safe_and_limited<optional<T>,maxSize>::number_of_queueing_portions.acquire();
         queue_safe_and_limited<optional<T>,maxSize>::number_of_queueing_portions.release();

         // check the current optional<T>, but don't pop it!
         lock_guard<mutex> lock(queue_safe_and_limited<optional<T>,maxSize>::m);
         return ! queue< optional<T> >::front().has_value();
     }
};

#endif // TSQUEUE_HPP
