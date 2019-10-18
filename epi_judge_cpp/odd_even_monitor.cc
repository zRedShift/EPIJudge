#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

using std::ref;
using std::cout;
using std::endl;
using std::thread;
using std::mutex;
using std::scoped_lock;
using std::unique_lock;
using std::condition_variable;

enum Turn { Even, Odd };
class OddEvenMonitor {
 public:
  void WaitTurn(Turn turn) {
    unique_lock lock{mu_};
    cond_.wait(lock, [&] { return turn == turn_; });
  }

  void ToggleTurn() {
    scoped_lock lock{mu_};
    turn_ = turn_ == Even ? Odd : Even;
    cond_.notify_one();
  }

 private:
  mutex mu_;
  condition_variable cond_;
  Turn turn_ = Odd;
};

void OddThread(OddEvenMonitor &monitor) {
  for (int i = 1; i <= 100; i += 2) {
    monitor.WaitTurn(Odd);
    cout << i << endl;
    monitor.ToggleTurn();
  }
}

void EvenThread(OddEvenMonitor &monitor) {
  for (int i = 2; i <= 100; i += 2) {
    monitor.WaitTurn(Even);
    cout << i << endl;
    monitor.ToggleTurn();
  }
}

int main(int argc, char *argv[]) {
  OddEvenMonitor monitor;
  thread t1(OddThread, ref(monitor));
  thread t2(EvenThread, ref(monitor));
  t1.join();
  t2.join();
  return 0;
}
