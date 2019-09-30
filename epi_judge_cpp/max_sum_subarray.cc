#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

int FindMaximumSubarray(const vector<int> &A) {
  int min_sum = 0, max_sum = 0, sum = 0;
  for (const auto x : A) {
    sum += x;
    if (sum < min_sum)
      min_sum = sum;
    if (sum - min_sum > max_sum)
      max_sum = sum - min_sum;
  }
  return max_sum;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(args, "max_sum_subarray.cc", "max_sum_subarray.tsv",
                         &FindMaximumSubarray, DefaultComparator{},
                         param_names);
}
