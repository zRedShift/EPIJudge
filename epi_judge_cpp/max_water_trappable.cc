#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

int CalculateTrappingWater(const vector<int> &heights) {
  int water = 0, left = 0, right = heights.size() - 1, max_left = heights[left], max_right = heights[right];
  while (left < right)
    if (max_left < max_right)
      max_left = std::max(max_left, heights[++left]), water += max_left - heights[left];
    else
      max_right = std::max(max_right, heights[--right]), water += max_right - heights[right];
  return water;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"heights"};
  return GenericTestMain(args, "max_water_trappable.cc",
                         "max_water_trappable.tsv", &CalculateTrappingWater,
                         DefaultComparator{}, param_names);
}
