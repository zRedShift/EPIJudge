#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

int CountInversionsHelper(vector<int> &A, vector<int> &B, int start, int end) {
  if (end - start < 2)
    return 0;
  int mid = start + (end - start) / 2, start2 = mid, curr = start;
  int count = CountInversionsHelper(A, B, start, mid) + CountInversionsHelper(A, B, mid, end);
  if (A[mid - 1] <= A[mid])
    return count;
  std::copy(&A[start], &A[end], &B[start]);
  while (start < mid && start2 < end)
    B[start] > B[start2] ? count += mid - start, A[curr++] = B[start2++] : A[curr++] = B[start++];
  while (start < mid)
    A[curr++] = B[start++];
  while (start2 < end)
    A[curr++] = B[start2++];
  return count;
}

int CountInversions(vector<int> A) {
  vector<int> B(A.size());
  return CountInversionsHelper(A, B, 0, A.size());
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(args, "count_inversions.cc", "count_inversions.tsv",
                         &CountInversions, DefaultComparator{}, param_names);
}
