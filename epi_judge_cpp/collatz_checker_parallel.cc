#include "test_framework/generic_test.h"

bool TestCollatzConjecture(int start, int finish) {
  std::unordered_set<size_t> temp;
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

bool TestCollatzConjectureWrapper(int n) {
  static const int num_threads = std::max(std::thread::hardware_concurrency(), 2u);
  const int adder = (n - 3) / num_threads;
  if (adder < num_threads)
    return TestCollatzConjecture(3, n);
  std::vector<std::future<bool>> futures;
  futures.reserve(num_threads);
  for (int start = 3, finish = start + adder + (n - 3) % num_threads; start < n; start = finish, finish += adder)
    futures.emplace_back(std::async(std::launch::async, TestCollatzConjecture, start, finish));
  return std::all_of(futures.begin(), futures.end(), [](auto &f) { return f.get(); });
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"n"};
  return GenericTestMain(args, "collatz_checker.cc", "collatz_checker.tsv",
                         &TestCollatzConjectureWrapper, DefaultComparator{},
                         param_names);
}
