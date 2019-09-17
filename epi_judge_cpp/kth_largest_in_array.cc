#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

int Partition(vector<int> &A, int lo, int hi) {
  int p = A[lo + (hi - lo) / 2];
  while (true) {
    while (A[lo] < p)
      ++lo;
    while (A[hi] > p)
      --hi;
    if (lo >= hi)
      return hi;
    std::swap(A[lo++], A[hi--]);
  }
}

void IsolateKSmallest(vector<int> &A, int lo, int hi, int k) {
  int p = Partition(A, lo, hi);
  if (int diff = k - p + lo - 1; diff < 0)
    IsolateKSmallest(A, lo, p, k);
  else if (diff > 0)
    IsolateKSmallest(A, p + 1, hi, diff);
}

void IsolateKLargest(vector<int> &A, int lo, int hi, int k) {
  int p = Partition(A, lo, hi);
  if (int diff = k - hi + p; diff < 0)
    IsolateKLargest(A, p + 1, hi, k);
  else if (diff > 0)
    IsolateKLargest(A, lo, p, diff);
}

// The numbering starts from one, i.e., if A = [3, 1, -1, 2] then
// FindKthLargest(1, A) returns 3, FindKthLargest(2, A) returns 2,
// FindKthLargest(3, A) returns 1, and FindKthLargest(4, A) returns -1.
int FindKthLargest(int k, vector<int>* A_ptr) {
  auto A = *A_ptr;
  if (2 * k > A.size()) {
    k = A.size() - k + 1;
    IsolateKSmallest(A, 0, A.size() - 1, k);
    return *std::max_element(A.begin(), A.begin() + k);
  }
  IsolateKLargest(A, 0, A.size() - 1, k);
  return *std::min_element(A.end() - k, A.end());
}
int FindKthLargestWrapper(int k, vector<int>& A) {
  return FindKthLargest(k, &A);
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"k", "A"};
  return GenericTestMain(args, "kth_largest_in_array.cc",
                         "kth_largest_in_array.tsv", &FindKthLargestWrapper,
                         DefaultComparator{}, param_names);
}
