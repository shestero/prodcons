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
template<typename T> class queue_thread_safe : public buffer<T> {
    template<typename> friend class queue_with_end;

public:
    queue_thread_safe() {
        m_overdry.lock();   // at the begining q is empty
    }

    virtual void put(const T& e) {
        lock_guard<mutex> lock(m_safe);
        q.push(e);
        m_overdry.unlock(); // there is something in q now
    }

    virtual void get(T& e) {
        m_overdry.lock();   // ensure if there is something in q
        lock_guard<mutex> lock(m_safe);
        e = q.front();
        q.pop();
        if (!q.empty())     // if there is notging in q then to prevent reading keep m_overdry locked
            m_overdry.unlock();
    }

protected:
    queue<T> q;
    mutex m_safe;           // made the access to q thread-safe
    mutex m_overdry;        // blocked when queue is empty;
};


// Queue that support end-of-data feature ( set_end() and bool end() )
// queue_with_end<T> wraps queue_with_end< optional<T> >
template <typename T> class queue_with_end : public buffer<T> {
public:
     virtual void put(const T& e) {
         q.put(optional(e));
     }
     virtual void get(T& e) {
         optional<T> op;
         q.get(op);
         if (op.has_value()) { // do nothinh if it is End-Of-Data
            e = op.value();
         }
     }

     virtual void set_end() {
         q.put(nullopt); // (optional<T>());
     }
     virtual bool end() {
         // check the current optional<T>, but don't pop it!
         lock_guard<mutex> lock(q.m_safe);
         if (q.q.empty())
            return false;
         else
            return ! q.q.front().has_value();
     }

protected:
     queue_thread_safe< optional<T> > q;
};

#endif // TSQUEUE_HPP
