#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

int SearchFirstOfK(const vector<int> &A, int k) {
  int from = 0, to = A.size() - 1, result = -1;
  while (from <= to)
    if (int mid = from + (to - from) / 2; A[mid] < k)
      from = mid + 1;
    else if (A[mid] > k)
      to = mid - 1;
    else
      result = mid, to = mid - 1;
  return result;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A", "k"};
  return GenericTestMain(args, "search_first_key.cc", "search_first_key.tsv",
                         &SearchFirstOfK, DefaultComparator{}, param_names);
}
