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
#include <mutex>
#include "semaphore.h" // C++17 simpliest binary semaphore

using namespace std;

// Thread-safe FIFO buffer-queue
template<typename T> class safe_buffer {
public:
    void put(const T& e) {
        lock_guard<mutex> lock(m);
        q.push(e);
        s.unlock();
    }

    void get(T& e) {
        s.lock();   // ensure if there is something in q
        lock_guard<mutex> lock(m);
        assert(!q.empty());
        e = q.front();
        q.pop();
        if (!q.empty())
        {
            // still have more data in queue
            s.unlock();
        }
    }

protected:
    queue<T> q;
    mutex m; // to made the access to q thread-safe
    binary_semaphore s{false}; // to block a consumer if the queue q is empty; initially locked
};



#endif // TSQUEUE_HPP
