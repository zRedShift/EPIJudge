#include <vector>
#include "test_framework/generic_test.h"
using std::vector;
bool CanReachEnd(const vector<int> &max_advance_steps) {
  size_t j = 0, last = max_advance_steps.size() - 1;
  for (size_t i = 0; i <= j && j < last; ++i)
    j = std::max(j, i + max_advance_steps[i]);
  return j >= last;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"max_advance_steps"};
  return GenericTestMain(args, "advance_by_offsets.cc",
                         "advance_by_offsets.tsv", &CanReachEnd,
                         DefaultComparator{}, param_names);
}
