#include <vector>
#include "test_framework/generic_test.h"
using std::vector;
double BuyAndSellStockTwiceAlternative(const vector<double> &prices) {
  double ext = prices.front(), prev = 0;
  vector<double> best_trades(prices.size());
  for (int i = 1; i < prices.size(); ++i) {
    prev = best_trades[i] = std::max(prev, prices[i] - ext);
    ext = std::min(ext, prices[i]);
  }
  ext = prices.back(), prev = 0;
  double best = best_trades.back();
  for (int j = prices.size() - 2; j > -1; --j) {
    prev = std::max(prev, ext - prices[j]);
    best = std::max(best, best_trades[j] + prev);
    ext = std::max(ext, prices[j]);
  }
  return best;
}

double BuyAndSellStockTwice(const vector<double> &prices) {
  std::pair<double, double> min{std::numeric_limits<double>::max(), std::numeric_limits<double>::max()}, best{0.0, 0.0};
  for (const auto price : prices) {
    best.first = std::max(best.first, price - min.first);
    best.second = std::max(best.second, price - min.second);
    min.first = std::min(min.first, price);
    min.second = std::min(min.second, price - best.first);
  }
  return best.second;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"prices"};
  return GenericTestMain(args, "buy_and_sell_stock_twice.cc",
                         "buy_and_sell_stock_twice.tsv", &BuyAndSellStockTwice,
                         DefaultComparator{}, param_names);
}
