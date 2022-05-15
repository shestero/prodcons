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
#include <mutex>
#include "semaphore.h" // C++17 simpliest binary semaphore


using namespace std;

// Thread-safe FIFO buffer-queue
template<typename T> class queue_thread_safe {
public:
    void put(const T& e) {
        lock_guard<mutex> lock(m);
        q.push(e);
    }

    // Warning: this get() don't chek bounds; it produces run-time error if is called on empty queue!
    // return true if the queue still not empty after the getting.
    bool get(T& e) {
        lock_guard<mutex> lock(m);
        assert(!q.empty());
        e = q.front();
        q.pop();
        return !q.empty();
    }

protected:
    queue<T> q;
    mutex m; // made the access to q thread-safe
};


// Queue that block getting while it is empty
template <typename T> class buffer_with_block  {
public:
     void put(const T& e) {
        q.put(e);
        s.unlock(); // there is something in q now
     }
     void get(T& e) {
        s.lock();   // ensure if there is something in q
        if (q.get(e))
        {
            // still have more data in queue
            s.unlock();
        }
     }

protected:
     queue_thread_safe< T > q;
     binary_semaphore s{false}; // to block cinsuming if the queue q is empty; initially locked
};

#endif // TSQUEUE_HPP
