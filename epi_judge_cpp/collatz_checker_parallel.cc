#include <vector>
#include "test_framework/generic_test.h"
using std::unordered_set;
using std::vector;
using std::thread;
using std::future;
using std::packaged_task;

bool TestCollatzConjecture(int start, int finish) {
  unordered_set<size_t> set, temp;
  for (size_t i = start; i <= finish; i += 2) {
    size_t k = i;
    do {
      if (!set.emplace(k).second)
        break;
      if (!temp.emplace(k).second)
        return false;
      k = (3 * k + 1) / 2;
      while (k % 2 == 0)
        k /= 2;
    } while (k >= i);
    temp.clear();
  }
  return true;
}

bool TestCollatzConjectureWrapper(int n) {
  static unsigned int num_threads = std::max(thread::hardware_concurrency(), 2u);
  unsigned int finish = n % (num_threads - 1), adder = n > num_threads ? n / (num_threads - 1) : 0;
  vector<future<bool>> futures;
  futures.reserve(num_threads);
  for (unsigned int start = 1; start < n; start = finish, finish += adder) {
    packaged_task<decltype(TestCollatzConjecture)> task{TestCollatzConjecture};
    futures.emplace_back(task.get_future());
    thread(std::move(task), start, finish).detach();
  }
  return std::all_of(futures.begin(), futures.end(), [](auto &f) { return f.get(); });
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"n"};
  return GenericTestMain(args, "collatz_checker.cc", "collatz_checker.tsv",
                         &TestCollatzConjectureWrapper, DefaultComparator{},
                         param_names);
}
