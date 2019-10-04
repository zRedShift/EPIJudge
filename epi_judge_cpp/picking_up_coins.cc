#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

int MaximumRevenue(const vector<int> &coins) {
  if (coins.size() <= 1)
    return coins.empty() ? 0 : coins.front();
  vector<int> loss(coins.size());
  for (int k = 1; k < coins.size() - 1; ++k)
    for (int i = 0, j = k; j < coins.size(); ++i, ++j)
      loss[i] = std::min(coins[i] + loss[i + 1], coins[j] + loss[i]);
  return std::max(coins.front() + loss[1], coins.back() + loss.front());
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"coins"};
  return GenericTestMain(args, "picking_up_coins.cc", "picking_up_coins.tsv",
                         &MaximumRevenue, DefaultComparator{}, param_names);
}
