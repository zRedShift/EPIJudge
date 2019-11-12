#include <vector>
#include "test_framework/generic_test.h"
using std::vector;
const int max_int = std::numeric_limits<int>::max(), min_int = std::numeric_limits<int>::min();

int FindKthInTwoSortedArrays(const vector<int> &A, const vector<int> &B, int k) {
  int l = std::max(0, static_cast<int>(k - B.size())), h = std::min(k, static_cast<int>(A.size()));
  while (l < h) {
    int m = l + (h - l) / 2;
    if (int A_m = m >= A.size() ? max_int : A[m], B_k_m_1 = k - m <= 0 ? min_int : B[k - m - 1]; A_m < B_k_m_1)
      l = m + 1;
    else if (int A_m_1 = m <= 0 ? min_int : A[m - 1], B_k_m = k - m >= B.size() ? max_int : B[k - m]; A_m_1 > B_k_m)
      h = m - 1;
    else
      return std::max(A_m_1, B_k_m_1);
  }
  return l > 0 ? k - l > 0 ? std::max(A[l - 1], B[k - l - 1]) : A[l - 1] : B[k - l - 1];
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A", "B", "k"};
  return GenericTestMain(args, "kth_largest_element_in_two_sorted_arrays.cc",
                         "kth_largest_element_in_two_sorted_arrays.tsv",
                         &FindKthInTwoSortedArrays, DefaultComparator{},
                         param_names);
}
