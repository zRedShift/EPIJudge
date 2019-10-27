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
  double min = std::numeric_limits<double>::max(), best_single = 0.0, max_leftover = -min, best = 0.0;
  for (const auto price : prices) {
    min = std::min(min, price);
    best_single = std::max(best_single, price - min);
    max_leftover = std::max(max_leftover, best_single - price);
    best = std::max(best, price + max_leftover);
  }
  return best;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"prices"};
  return GenericTestMain(args, "buy_and_sell_stock_twice.cc",
                         "buy_and_sell_stock_twice.tsv", &BuyAndSellStockTwice,
                         DefaultComparator{}, param_names);
}
