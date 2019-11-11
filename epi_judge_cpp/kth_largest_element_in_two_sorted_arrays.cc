#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

int FindKthInTwoSortedArrays(const vector<int> &A, const vector<int> &B,
                             int k) {
  auto A_start = A.begin(), A_end = std::min(A.end(), A_start + k), B_start = B.begin(),
      B_end = std::min(B.end(), B_start + k);
  int A_size = std::min(int(A.size()), k), B_size = std::min(int(B.size()), k), size = A_size + B_size;
  while (k != 1 && size != k && A_size && B_size) {
    vector<int>::const_iterator A_mid, B_mid;
    if (A_size > B_size)
      B_mid = std::lower_bound(B_start, B_end, *(A_mid = A_start + A_size / 2));
    else
      A_mid = std::lower_bound(A_start, A_end, *(B_mid = B_start + B_size / 2));
    if (int A_new = std::distance(A_start, A_mid), B_new = std::distance(B_start, B_mid); A_new + B_new < k)
      k -= A_new + B_new, A_size -= A_new, B_size -= B_new, A_start = A_mid, B_start = B_mid;
    else
      A_size = A_new, B_size = B_new, A_end = A_mid, B_end = B_mid;
    size = A_size + B_size;
  }
  if (!A_size || !B_size)
    return A_size ? *(A_start + k - 1) : *(B_start + k - 1);
  return k == 1 ? std::min(*A_start, *B_start) : std::max(*(A_end - 1), *(B_end - 1));
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A", "B", "k"};
  return GenericTestMain(args, "kth_largest_element_in_two_sorted_arrays.cc",
                         "kth_largest_element_in_two_sorted_arrays.tsv",
                         &FindKthInTwoSortedArrays, DefaultComparator{},
                         param_names);
}
