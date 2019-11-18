#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

int FindFirstMissingPositive(vector<int> &A) {
  for (int i = 0; i < A.size(); ++i)
    while (A[i] > 0 && A[i] <= A.size() && A[i] != A[A[i] - 1])
      std::swap(A[i], A[A[i] - 1]);
  for (int i = 0; i < A.size(); ++i)
    if (A[i] != i + 1)
      return i + 1;
  return A.size() + 1;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(args, "first_missing_positive_entry.cc",
                         "first_missing_positive_entry.tsv",
                         &FindFirstMissingPositive, DefaultComparator{},
                         param_names);
}
