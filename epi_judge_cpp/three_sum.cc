#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

bool HasThreeSum(vector<int> A, int t) {
  std::sort(A.begin(), A.end());
  for (int k = 0; k < A.size(); ++k) {
    int tt = t - A[k], i = k, j = A.size() - 1;
    while (i <= j)
      if (int sum = A[i] + A[j]; sum == tt)
        return true;
      else if (sum > tt)
        --j;
      else
        ++i;
  }
  return false;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A", "t"};
  return GenericTestMain(args, "three_sum.cc", "three_sum.tsv", &HasThreeSum,
                         DefaultComparator{}, param_names);
}
