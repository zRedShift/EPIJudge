#include <vector>
#include "test_framework/generic_test.h"
using std::unordered_set;
using std::vector;

bool TestCollatzConjecture(int n) {
  unordered_set<size_t> temp;
  for (size_t i = 3; i <= n; i += 2) {
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

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"n"};
  return GenericTestMain(args, "collatz_checker.cc", "collatz_checker.tsv",
                         &TestCollatzConjecture, DefaultComparator{},
                         param_names);
}
