#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

int CountInversions(const vector<int> &A) {
  int count = 0;
  for (int i = 0; i < A.size(); ++i)
    for (int j = i + 1; j < A.size(); ++j)
      count += A[i] > A[j];
  return count;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(args, "count_inversions.cc", "count_inversions.tsv",
                         &CountInversions, DefaultComparator{}, param_names);
}
