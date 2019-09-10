#include <vector>
#include "test_framework/generic_test.h"
using std::vector;
vector<vector<int>> GeneratePascalTriangle(int num_rows) {
  vector<vector<int>> vec(num_rows);
  for (int i = 0; i < num_rows; ++i) {
    vec[i].resize(i + 1, 1);
    for (int j = 1; j < i; ++j)
      vec[i][j] = vec[i - 1][j - 1] + vec[i - 1][j];
  }
  return vec;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"num_rows"};
  return GenericTestMain(args, "pascal_triangle.cc", "pascal_triangle.tsv",
                         &GeneratePascalTriangle, DefaultComparator{},
                         param_names);
}
