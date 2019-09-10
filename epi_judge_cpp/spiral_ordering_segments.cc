#include <vector>
#include "test_framework/generic_test.h"
using std::vector;
using std::for_each;
vector<int> MatrixInSpiralOrder(vector<vector<int>> square_matrix) {
  int n = size(square_matrix), center = n / 2;
  vector<int> result;
  result.reserve(n * n);
  for (int m = 0; m < center; ++m) {
    auto top = square_matrix.begin() + m, bot = square_matrix.end() - m - 1;
    auto rbot = square_matrix.rbegin() + m, rtop = square_matrix.rend() - m - 1;
    result.insert(result.end(), top->begin() + m, top->end() - m);
    for_each(top + 1, bot, [&result, m](auto &x) { result.emplace_back(*(x.end() - m - 1)); });
    result.insert(result.end(), rbot->rbegin() + m, rbot->rend() - m);
    for_each(rbot + 1, rtop, [&result, m](auto &x) { result.emplace_back(*(x.begin() + m)); });
  }
  if (n % 2)
    result.emplace_back(square_matrix[center][center]);
  return result;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"square_matrix"};
  return GenericTestMain(args, "spiral_ordering_segments.cc",
                         "spiral_ordering_segments.tsv", &MatrixInSpiralOrder,
                         DefaultComparator{}, param_names);
}
