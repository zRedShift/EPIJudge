#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

int LongestContainedRange(const vector<int> &A) {
  std::unordered_set<int> appearances(A.begin(), A.end());
  int max = 1;
  for (auto iter = appearances.begin(); iter != appearances.end(); appearances.erase(*iter++)) {
    int lower = *iter - 1, upper = *iter + 1;
    while (appearances.count(lower))
      appearances.erase(lower--);
    while (appearances.count(upper))
      appearances.erase(upper++);
    max = std::max(max, upper - lower - 1);
  }
  return max;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(
      args, "longest_contained_interval.cc", "longest_contained_interval.tsv",
      &LongestContainedRange, DefaultComparator{}, param_names);
}
