#include <vector>
#include "test_framework/generic_test.h"
using std::swap;
using std::vector;

vector<int> ApplyPermutation(vector<int> perm, vector<int> vec) {
  for (int p = 0; p < static_cast<int>(size(perm)); ++p)
    for (int curr = perm[p]; p != curr;)
      swap(vec[p], vec[curr]), swap(curr, perm[curr]);
  return vec;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"perm", "A"};
  return GenericTestMain(args, "apply_permutation.cc", "apply_permutation.tsv",
                         &ApplyPermutation, DefaultComparator{},
                         param_names);
}
