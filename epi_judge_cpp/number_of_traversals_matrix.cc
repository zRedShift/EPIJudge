#include "test_framework/generic_test.h"

int NumberOfWays(int n, int m) {
  if (n < m)
    std::swap(n, m);
  std::vector<int> row(m, 1);
  for (int i = 1; i < n; ++i)
    for (int j = 1; j < m; ++j)
      row[j] += row[j - 1];
  return row.back();
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"n", "m"};
  return GenericTestMain(args, "number_of_traversals_matrix.cc",
                         "number_of_traversals_matrix.tsv", &NumberOfWays,
                         DefaultComparator{}, param_names);
}
