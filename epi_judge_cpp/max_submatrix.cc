#include <deque>
#include <vector>
#include "test_framework/generic_test.h"
using std::deque;
using std::vector;

int MaxRectangleSubmatrix(const vector<deque<bool>> &A) {
  vector<int> cache(A.front().size()), stack;
  int result = 0, height;
  for (const auto &row : A) {
    for (int i = 0; i < cache.size(); stack.push_back(i++)) {
      int curr = ++cache[i] *= row[i];
      while (!stack.empty() && (height = cache[stack.back()]) >= curr)
        stack.pop_back(), result = std::max(result, height * (i - (stack.empty() ? 0 : stack.back() + 1)));
    }
    while (!stack.empty())
      height = cache[stack.back()], stack.pop_back(), result =
          std::max(result, height * static_cast<int>(cache.size() - (stack.empty() ? 0 : stack.back() + 1)));
  }
  return result;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(args, "max_submatrix.cc", "max_submatrix.tsv",
                         &MaxRectangleSubmatrix, DefaultComparator{},
                         param_names);
}
