#include <vector>
#include "test_framework/generic_test.h"
using std::unordered_set;
using std::vector;

bool TestCollatzConjecture(int n) {
  unordered_set<size_t> set{1}, temp;
  for (size_t i = 3; i <= n; i += 2) {
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

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"n"};
  return GenericTestMain(args, "collatz_checker.cc", "collatz_checker.tsv",
                         &TestCollatzConjecture, DefaultComparator{},
                         param_names);
}
