#include <vector>
#include "test_framework/generic_test.h"
using std::vector;
vector<int> NextPermutation(vector<int> perm) {
  auto p = std::is_sorted_until(perm.rbegin(), perm.rend());
  if (p == perm.rend())
    return {};
  std::iter_swap(p, std::upper_bound(perm.rbegin(), p, *p));
  std::reverse(perm.rbegin(), p);
  return perm;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"perm"};
  return GenericTestMain(args, "next_permutation.cc", "next_permutation.tsv",
                         &NextPermutation, DefaultComparator{}, param_names);
}
