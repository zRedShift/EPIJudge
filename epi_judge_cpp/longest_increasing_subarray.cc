#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

struct Subarray {
  int start = 0, end = 0;
};

Subarray FindLongestIncreasingSubarray(const vector<int> &A) {
  int start = 0, len = 1, max_len = 0;
  for (int i = 1; i < A.size(); ++i) {
    if (A[i] <= A[i - 1]) {
      if (len > max_len)
        start = i - len, max_len = len;
      len = 1;
    } else
      ++len;
  }
  if (len > max_len)
    start = A.size() - len, max_len = len;
  return {start, start + max_len - 1};
}
int FindLongestIncreasingSubarrayWrapper(const vector<int> &A) {
  Subarray result = FindLongestIncreasingSubarray(A);
  return result.end - result.start + 1;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(
      args, "longest_increasing_subarray.cc", "longest_increasing_subarray.tsv",
      &FindLongestIncreasingSubarrayWrapper, DefaultComparator{}, param_names);
}
