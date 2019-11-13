#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

int FindBiggestNMinusOneProduct(const vector<int> &A) {
  vector<int> B(A.size() - 1);
  std::partial_sum(A.cbegin(), A.cend() - 1, B.begin(), std::multiplies<>());
  int max = B.back(), x = A.back();
  for (int i = B.size() - 2; i > -1; --i)
    max = std::max(max, B[i] * x), x *= A[i + 1];
  return std::max(max, x);
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(
      args, "max_product_all_but_one.cc", "max_product_all_but_one.tsv",
      &FindBiggestNMinusOneProduct, DefaultComparator{}, param_names);
}
