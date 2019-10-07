#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

int CalculateLargestRectangle(const vector<int> &heights) {
  std::stack<int, vector<int>> candidates;
  int result = 0;
  for (int i = 0; i < heights.size(); ++i) {
    int curr = heights[i], height;
    while (!candidates.empty() && (height = heights[candidates.top()]) >= curr) {
      candidates.pop();
      int width = candidates.empty() ? i : i - candidates.top() - 1;
      result = std::max(result, height * width);
    }
    candidates.emplace(i);
  }
  while (!candidates.empty()) {
    int height = heights[candidates.top()];
    candidates.pop();
    int width = candidates.empty() ? heights.size() : heights.size() - candidates.top() - 1;
    result = std::max(result, height * width);
  }
  return result;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"heights"};
  return GenericTestMain(args, "largest_rectangle_under_skyline.cc",
                         "largest_rectangle_under_skyline.tsv",
                         &CalculateLargestRectangle, DefaultComparator{},
                         param_names);
}
