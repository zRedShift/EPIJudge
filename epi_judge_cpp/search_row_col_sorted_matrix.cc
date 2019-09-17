#include <vector>
#include "test_framework/generic_test.h"
using std::vector;
bool MatrixSearch(const vector<vector<int>> &A, int k) {
  int row = 0, col = A[0].size() - 1;
  while (row < A.size() && col >= 0)
    if (A[row][col] < k)
      ++row;
    else if (A[row][col] > k)
      --col;
    else
      return true;
  return false;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A", "x"};
  return GenericTestMain(args, "search_row_col_sorted_matrix.cc",
                         "search_row_col_sorted_matrix.tsv", &MatrixSearch,
                         DefaultComparator{}, param_names);
}
