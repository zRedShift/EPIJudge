#include <chrono>
#include <thread>
#include <condition_variable>
#include <iostream>
using std::mutex;
using std::unique_lock;
using std::condition_variable;
using std::string;
using std::ref;
using std::thread;
using std::this_thread::sleep_for;
using std::cout;
using std::endl;
using std::chrono::milliseconds;

class Semaphore {
 public:
  explicit Semaphore(int max_available) : max_available_(max_available), taken_(0) {}
  void Acquire() {
    unique_lock lock{mutex_};
    cond_.wait(lock, [&] { return taken_ != max_available_; });
    ++taken_;
  }
  void Release() {
    unique_lock lock{mutex_};
    if (taken_-- == max_available_)
      cond_.notify_one();
  }
  int NumTaken() {
    unique_lock lock{mutex_};
    return taken_;
  }
 private:
  int max_available_, taken_;
  mutex mutex_;
  condition_variable cond_;
};

static int shared_data = 0;

void IncrementThread(Semaphore &semaphore, const string &name) {
  while (true) {
    cout << name << " is waiting for permit" << endl;
    semaphore.Acquire();
    cout << name << " has got permit, new count is " << semaphore.NumTaken()
         << endl;
    for (int i = 0; i < 3; ++i) {
      sleep_for(milliseconds(200));
      cout << name + " increments shared data: " << shared_data++ << endl;
    }
    cout << name + " has released permit" << endl;
    semaphore.Release();
  }
}

void DecrementThread(Semaphore &semaphore, const string &name) {
  while (true) {
    cout << name << " is waiting for permit" << endl;
    semaphore.Acquire();
    cout << name << " has got permit, new count is " << semaphore.NumTaken()
         << endl;
    for (int i = 0; i < 4; ++i) {
      sleep_for(milliseconds(300));
      cout << name + " decrements shared data: " << shared_data-- << endl;
    }
    cout << name + " has released permit" << endl;
    semaphore.Release();
  }
}

int main(int argc, char *argv[]) {
  Semaphore semaphore(1);
  cout << "Semaphore with 1 permit has been created" << endl;
  thread increment_thread1(IncrementThread, ref(semaphore),
                           "increment_thread_1");
  thread decrement_thread1(DecrementThread, ref(semaphore),
                           "decrement_thread_1");
  thread increment_thread2(IncrementThread, ref(semaphore),
                           "increment_thread_2");
  thread decrement_thread2(DecrementThread, ref(semaphore),
                           "decrement_thread_2");
  increment_thread1.join();
}
