#include "test_framework/generic_test.h"

int ComputeBinomialCoefficient(int n, int k) {
  k = std::min(k, n - k) + 1, n -= k - 2;
  std::vector<int> row(k, 1);
  for (int i = 1; i < n; ++i)
    for (int j = 1; j < k; ++j)
      row[j] += row[j - 1];
  return row.back();
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"n", "k"};
  return GenericTestMain(
      args, "binomial_coefficients.cc", "binomial_coefficients.tsv",
      &ComputeBinomialCoefficient, DefaultComparator{}, param_names);
}
