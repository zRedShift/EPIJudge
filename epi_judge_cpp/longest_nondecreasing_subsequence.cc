#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

int LongestNondecreasingSubsequenceLength(const vector<int> &A) {
  vector<int> tails{std::numeric_limits<int>::max()};
  for (const auto x : A)
    if (tails.back() <= x)
      tails.emplace_back(x);
    else
      *std::upper_bound(tails.begin(), tails.end(), x) = x;
  return tails.size();
}

int LongestNondecreasingSubsequenceLengthAlternative(const vector<int> &A) {
  vector<int> lengths(A.size(), 1);
  int result = 1;
  for (int i = 0; i < A.size(); ++i)
    for (int j = i + 1; j < A.size(); ++j)
      result = std::max(result, lengths[j] = std::max(lengths[j], (A[i] <= A[j]) * (lengths[i] + 1)));
  return result;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(args, "longest_nondecreasing_subsequence.cc",
                         "longest_nondecreasing_subsequence.tsv",
                         &LongestNondecreasingSubsequenceLength,
                         DefaultComparator{}, param_names);
}
