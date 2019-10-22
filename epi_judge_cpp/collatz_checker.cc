#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

bool TestCollatzConjecture(int n) {
  if (n < 2)
    return true;
  static vector<bool> tested(std::numeric_limits<unsigned>::max(), false);
  vector<unsigned> temp;
  tested[1] = true;
  for (unsigned i = 2; i <= n; ++i) {
    if (tested[i])
      continue;
    if (i % 2 == 0)
      tested[i] = tested[i / 2];
    else {
      unsigned k = 3 * i + 1;
      while (!tested[k])
        temp.push_back(k), k = k % 2 ? 3 * k + 1 : k / 2;
      for (const auto t : temp)
        tested[t] = true;
      temp.resize(0);
    }
    for (unsigned long j = i; j < std::numeric_limits<unsigned>::max(); j <<= 1)
      tested[j] = true;
  }
  return std::all_of(tested.begin() + 1, tested.begin() + n + 1, [](bool v) { return v; });
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"n"};
  return GenericTestMain(args, "collatz_checker.cc", "collatz_checker.tsv",
                         &TestCollatzConjecture, DefaultComparator{},
                         param_names);
}
