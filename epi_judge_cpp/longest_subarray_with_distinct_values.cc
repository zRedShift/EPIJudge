#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

int LongestSubarrayWithDistinctEntries(const vector<int> &A) {
  std::unordered_map<int, int> appearances;
  int left = 0, longest = 0;
  for (int right = 0; right < A.size(); ++right)
    if (auto &x = appearances[A[right]]; x++) {
      longest = std::max(longest, right - left);
      do {
        --appearances[A[left++]];
      } while (x > 1);
    }
  return std::max(longest, static_cast<int>(A.size() - left));
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(args, "longest_subarray_with_distinct_values.cc",
                         "longest_subarray_with_distinct_values.tsv",
                         &LongestSubarrayWithDistinctEntries,
                         DefaultComparator{}, param_names);
}