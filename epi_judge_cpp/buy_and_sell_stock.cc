#include <vector>
#include "test_framework/generic_test.h"

double BuyAndSellStockOnce(const std::vector<double> &prices) {
  double min = std::numeric_limits<double>::max(), max_diff{0.0};
  for (const auto& price : prices) {
    min = std::min(price, min);
    max_diff = std::max(price - min, max_diff);
  }
  return max_diff;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"prices"};
  return GenericTestMain(args, "buy_and_sell_stock.cc",
                         "buy_and_sell_stock.tsv", &BuyAndSellStockOnce,
                         DefaultComparator{}, param_names);
}
