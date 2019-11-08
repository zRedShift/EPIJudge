#include <deque>
#include <vector>
#include "test_framework/generic_test.h"
using std::deque;
using std::vector;

int MaxSquareSubmatrix(const vector<deque<bool>> &A) {
  vector<int> cache(A.front().size()), curr;
  int result = 0;
  for (const auto &row : A) {
    curr.assign(row.cbegin(), row.cend());
    for (int i = 1; i < cache.size(); ++i)
      curr[i] *= 1 + std::min({cache[i], cache[i - 1], curr[i - 1]});
    std::swap(curr, cache), result = std::max(result, *std::max_element(cache.begin(), cache.end()));
  }
  return result * result;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(args, "max_square_submatrix.cc",
                         "max_square_submatrix.tsv", &MaxSquareSubmatrix,
                         DefaultComparator{}, param_names);
}
