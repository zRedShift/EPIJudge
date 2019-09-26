#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

void ComputePermutations(vector<int> &permutation, int n, vector<vector<int>> &permutations) {
  if (n == permutation.size() - 1) {
    permutations.emplace_back(permutation);
    return;
  }
  const int next = n + 1;
  ComputePermutations(permutation, next, permutations);
  for (int i = next; i < permutation.size(); ++i) {
    std::swap(permutation[n], permutation[i]);
    ComputePermutations(permutation, next, permutations);
    std::swap(permutation[n], permutation[i]);
  }
}

vector<vector<int>> Permutations(vector<int> A) {
  size_t n = 1;
  for (size_t i = 2; i <= A.size(); ++i)
    n *= i;
  vector<vector<int>> result;
  result.reserve(n);
//  ComputePermutations(A, 0, result);
  sort(A.begin(), A.end());
  do
    result.push_back(A);
  while (std::next_permutation(A.begin(), A.end()));
  return result;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(
      args, "permutations.cc", "permutations.tsv", &Permutations,
      &UnorderedComparator<vector<vector<int>>>, param_names);
}
