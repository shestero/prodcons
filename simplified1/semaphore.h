/* binary_semaphore for C++17.
 * Note: binary_semaphore as well as semaphore are build in C++20 and later.
*/

#pragma once // non-standard but widely supported

#include <cassert>
#include <condition_variable>
#include <limits>
#include <mutex>

using namespace std;

class binary_semaphore {
public:
  binary_semaphore(bool unlocked) : condition(unlocked) {}

  void unlock() { // ex release 
    m.lock();
    condition = true;
    m.unlock();
    // why we lock mutex here for setting the boolean value (isn't it an atomic operation?)
    // we should do this to ensure of saving this value:
    // to care about if during multithreading execution this unblock() methid
    // is called just between cv.wait and 'condition=false;' below.

    cv.notify_all();
  }

  void lock() { // ex acquire
    unique_lock<mutex> lock(m);
    cv.wait(lock, [&]() { return condition; });
    condition=false;
  }

protected:
  bool condition; // false = locked; true = unlocked

  condition_variable cv;
  mutex m;
};
