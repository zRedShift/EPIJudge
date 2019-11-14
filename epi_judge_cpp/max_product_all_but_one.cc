#include <vector>
#include "test_framework/generic_test.h"
using std::multiplies;
using std::accumulate;

int FindBiggestNMinusOneProduct(const std::vector<int> &A) {
  std::vector<int> B(A.size() - 1);
  std::partial_sum(A.cbegin(), A.cend() - 1, B.begin(), std::multiplies<>());
  int max = B.back(), x = A.back();
  for (int i = B.size() - 2; i > -1; --i)
    max = std::max(max, B[i] * x), x *= A[i + 1];
  return std::max(max, x);
}

int FindBiggestNMinusOneProductAlt(const std::vector<int> &A) {
  int negative = 0, i = 0, min = std::numeric_limits<int>::max(), j = 0, max = std::numeric_limits<int>::min();
  for (int k = 0; k < A.size(); ++k)
    if (int x = A[k]; x < 0) {
      if (++negative; x > max)
        j = k, max = x;
    } else if (x < min)
      i = k, min = x;
  if (negative % 2)
    std::swap(i, j);
  return accumulate(&A[i + 1], &A[A.size()], accumulate(&A.front(), &A[i], 1, multiplies<>()), multiplies<>());
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(
      args, "max_product_all_but_one.cc", "max_product_all_but_one.tsv",
      &FindBiggestNMinusOneProduct, DefaultComparator{}, param_names);
}
