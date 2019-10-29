#include <vector>
#include "test_framework/generic_test.h"
using std::vector;
void RotateMatrix(vector<vector<int>> *square_matrix_ptr) {
  auto &m = *square_matrix_ptr;
  for (int i = 0, n = m.size() - 1; n > 0; n -= 2, ++i)
    for (int k = 0, l = n; k < n; ++k, --l) {
      const int tl = m[i][i + k], tr = m[i + k][i + n], bl = m[i + l][i], br = m[i + n][i + l];
      m[i][i + k] = bl, m[i + l][i] = br, m[i + n][i + l] = tr, m[i + k][i + n] = tl;
    }
}
vector<vector<int>> RotateMatrixWrapper(vector<vector<int>> square_matrix) {
  RotateMatrix(&square_matrix);
  return square_matrix;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"square_matrix"};
  return GenericTestMain(args, "matrix_rotation.cc", "matrix_rotation.tsv",
                         &RotateMatrixWrapper, DefaultComparator{},
                         param_names);
}
