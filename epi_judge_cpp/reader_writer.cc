#include <atomic>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <random>
#include <string>
#include <thread>

using std::string;
using std::cout;
using std::endl;
using std::random_device;
using std::default_random_engine;
using std::uniform_int_distribution;
using std::chrono::milliseconds;
using std::thread;
using std::this_thread::sleep_for;
using std::mutex;
using std::atomic;
using std::scoped_lock;
using std::unique_lock;
using std::condition_variable;

namespace RW {
int data = 0;
condition_variable LR;
atomic<int> read_count = 0;
mutex LW;
}

void DoSomethingElse() {
  static default_random_engine rnd((random_device()) ());
  uniform_int_distribution<> wait_time(0, 1000);
  sleep_for(milliseconds(wait_time(rnd)));
}

void Reader(string name) {
  while (true) {
    {
      scoped_lock lock(RW::LW);
      ++RW::read_count;
    }
    cout << "Reader " << name << " is about to read" << endl;
    cout << RW::data << endl;
    if (!--RW::read_count)
      RW::LR.notify_one();
    DoSomethingElse();
  }
}

void Writer(string name) {
  while (true) {
    {
      unique_lock lock(RW::LW);
      RW::LR.wait(lock, [&] { return RW::read_count.load() == 0; });
      cout << "Writer " << name << " is about to write" << endl;
      ++RW::data;
    }
    DoSomethingElse();
  }
}

int main(int argc, char *argv[]) {
  thread r0(Reader, "r0");
  thread r1(Reader, "r1");
  thread w0(Writer, "w0");
  thread w1(Writer, "w1");
  sleep_for(milliseconds(10000));
  return 0;
}
