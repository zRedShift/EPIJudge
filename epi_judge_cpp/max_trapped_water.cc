#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

int GetMaxTrappedWater(const vector<int> &heights) {
  int i = 0, j = heights.size() - 1, trapped = 0;
  while (i < j)
    if (heights[i] < heights[j])
      trapped = std::max(trapped, heights[i] * (j - i)), ++i;
    else
      trapped = std::max(trapped, heights[j] * (j - i)), --j;
  return trapped;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"heights"};
  return GenericTestMain(args, "max_trapped_water.cc", "max_trapped_water.tsv",
                         &GetMaxTrappedWater, DefaultComparator{}, param_names);
}
