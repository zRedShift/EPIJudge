#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

int MaxSubarraySumInCircular(const vector<int> &A) {
  int min_sum = 0, sum = 0, max_sum = 0, max = 0, min = 0;
  for (const int &x : A)
    sum += x, min_sum = std::min(min_sum, sum), max_sum = std::max(max_sum, sum),
    max = std::max(max, sum - min_sum), min = std::min(min, sum - max_sum);
  return std::max(max, sum - min);
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(args, "maximum_subarray_in_circular_array.cc",
                         "maximum_subarray_in_circular_array.tsv",
                         &MaxSubarraySumInCircular, DefaultComparator{},
                         param_names);
}
