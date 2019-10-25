#include "test_framework/generic_test.h"
#include <boost/thread/sync_bounded_queue.hpp>

bool TestCollatzConjecture(std::unordered_set<size_t> &temp, int start, int finish) {
  for (size_t i = start; i <= finish; i += 2) {
    size_t k = i;
    do {
      if (!temp.emplace(k).second)
        return false;
      k = 3 * k + 1;
      do
        k >>= 1u;
      while ((k & 1u) == 0);
    } while (k >= i);
    temp.clear();
  }
  return true;
}

bool Executor(boost::sync_bounded_queue<std::pair<int, int>> &q) {
  std::pair<int, int> args;
  std::unordered_set<size_t> temp;
  while (q.wait_pull_front(args) != boost::queue_op_status::closed)
    if (!TestCollatzConjecture(temp, args.first, args.second))
      return false;
  return true;
}

bool TestCollatzConjectureWrapper(int n) {
  const int kRange = 100'000;
  if (n < 3)
    return true;
  if (n <= kRange)
    return TestCollatzConjecture(*std::make_unique<std::unordered_set<size_t>>().get(), 3, n);
  static const size_t kNumThreads = std::max(std::thread::hardware_concurrency(), 2u);
  boost::sync_bounded_queue<std::pair<int, int>> q{kNumThreads};
  std::vector<std::future<bool>> futures;
  futures.reserve(kNumThreads);
  for (int i = 0; i < kNumThreads; ++i)
    futures.emplace_back(std::async(std::launch::async, Executor, std::ref(q)));
  q << std::make_pair(3, kRange + n % kRange);
  for (int i = kRange + n % kRange; i < n; i += kRange)
    q << std::make_pair(i, i + kRange);
  q.close();
  return std::all_of(futures.begin(), futures.end(), [](auto &f) { return f.get(); });
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"n"};
  return GenericTestMain(args, "collatz_checker_parallel.cc", "collatz_checker_parallel.tsv",
                         &TestCollatzConjectureWrapper, DefaultComparator{},
                         param_names);
}
