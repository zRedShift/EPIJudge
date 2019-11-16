#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

int FindLongestSubarrayLessEqualK(const vector<int> &A, int k) {
  vector<std::pair<int, int>> max{{0, 0}}, rmax{{A.size(), 0}};
  int sum = 0, rsum = 0, length = 0;
  for (int i = 0, j = A.size() - 1; j > -1; ++i, --j) {
    sum += A[i], rsum += A[j];
    if (sum > max.back().second)
      max.emplace_back(i + 1, sum);
    if (rsum > rmax.back().second)
      rmax.emplace_back(j, rsum);
  }
  if (sum <= k)
    return A.size();
  for (int i = 0, j = rmax.size() - 1; i < max.size() && j > -1;)
    if (sum - rmax[j].second - max[i].second > k)
      ++i;
    else
      length = std::max(length, rmax[j].first - max[i].first), --j;
  return length;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A", "k"};
  return GenericTestMain(args, "longest_subarray_with_sum_constraint.cc",
                         "longest_subarray_with_sum_constraint.tsv",
                         &FindLongestSubarrayLessEqualK, DefaultComparator{},
                         param_names);
}
