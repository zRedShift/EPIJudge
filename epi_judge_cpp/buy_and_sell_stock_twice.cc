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
  double first_buy = std::numeric_limits<double>::max(), first_sell = 0.0, second_buy = -first_buy, second_sell = 0.0;
  for (const auto price : prices) {
    first_buy = std::min(first_buy, price);
    first_sell = std::max(first_sell, price - first_buy);
    second_buy = std::max(second_buy, first_sell - price);
    second_sell = std::max(second_sell, second_buy + price);
  }
  return second_sell;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"prices"};
  return GenericTestMain(args, "buy_and_sell_stock_twice.cc",
                         "buy_and_sell_stock_twice.tsv", &BuyAndSellStockTwice,
                         DefaultComparator{}, param_names);
}
