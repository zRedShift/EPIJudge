#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

int MaximumRevenue(const vector<int> &coins) {
  if (coins.size() <= 1)
    return coins.empty() ? 0 : coins.front();
  vector<std::pair<int, int>> tally(coins.size() - 1);
  std::transform(std::next(coins.cbegin()),
                 coins.cend(),
                 coins.cbegin(),
                 tally.begin(),
                 [](const int &x, const int &y) { return std::minmax(x, y); });
  for (int k = 2; k < coins.size(); ++k)
    for (int i = 0, j = k; j < coins.size(); ++i, ++j)
      if (int left = coins[i] + tally[i + 1].first, right = coins[j] + tally[i].first; left < right)
        tally[i].first = tally[i].second, tally[i].second = right;
      else
        tally[i].first = tally[i + 1].second, tally[i].second = left;
  return tally.front().second;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"coins"};
  return GenericTestMain(args, "picking_up_coins.cc", "picking_up_coins.tsv",
                         &MaximumRevenue, DefaultComparator{}, param_names);
}
