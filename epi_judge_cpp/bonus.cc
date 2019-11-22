#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

int CalculateBonus(const vector<int> &prod) {
  vector<int> vec(prod.size()), t(prod.size(), 1);
  std::iota(vec.begin(), vec.end(), 0);
  std::sort(vec.begin(), vec.end(), [&prod](const auto &lhs, const auto &rhs) { return prod[lhs] < prod[rhs]; });
  for (const int i : vec) {
    if (i > 0 && prod[i] > prod[i - 1])
      t[i] += t[i - 1];
    if (i + 1 < prod.size() && prod[i] > prod[i + 1])
      t[i] = std::max(t[i], t[i + 1] + 1);
  }
  return std::accumulate(t.begin(), t.end(), 0);
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"productivity"};
  return GenericTestMain(args, "bonus.cc", "bonus.tsv", &CalculateBonus,
                         DefaultComparator{}, param_names);
}
