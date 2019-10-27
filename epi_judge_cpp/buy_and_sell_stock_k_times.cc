#include <vector>
#include "test_framework/generic_test.h"
using std::max;
using std::numeric_limits;
using std::pair;
using std::vector;

double BuyAndSellStockKTimes(const vector<double> &prices, int k) {
  if (!k)
    return 0.0;
  if (2 * k >= prices.size()) {
    double profit = 0.0;
    for (int i = 1; i < prices.size(); ++i)
      profit += max(0.0, prices[i] - prices[i - 1]);
    return profit;
  }
  vector<pair<double, double>> buys_and_sells(k, {-numeric_limits<double>::max(), 0});
  for (const auto price : prices) {
    double prev_sell = 0.0;
    for (auto &buy_and_sell : buys_and_sells) {
      buy_and_sell.first = max(buy_and_sell.first, prev_sell - price);
      prev_sell = buy_and_sell.second = max(buy_and_sell.second, buy_and_sell.first + price);
    }
  }
  return buys_and_sells.back().second;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"prices", "k"};
  return GenericTestMain(
      args, "buy_and_sell_stock_k_times.cc", "buy_and_sell_stock_k_times.tsv",
      &BuyAndSellStockKTimes, DefaultComparator{}, param_names);
}
