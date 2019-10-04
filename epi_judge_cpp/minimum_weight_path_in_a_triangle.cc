#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

int MinimumPathWeight(const vector<vector<int>> &triangle) {
  if (triangle.empty())
    return 0;
  vector<int> result = triangle.back();
  for (int i = triangle.size() - 2; i >= 0; --i)
    for (int j = 0; j <= i; ++j)
      result[j] = std::min(result[j], result[j + 1]) + triangle[i][j];
  return result.front();
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"triangle"};
  return GenericTestMain(args, "minimum_weight_path_in_a_triangle.cc",
                         "minimum_weight_path_in_a_triangle.tsv",
                         &MinimumPathWeight, DefaultComparator{}, param_names);
}
