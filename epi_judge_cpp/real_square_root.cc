#include "test_framework/generic_test.h"

double SquareRoot(double k) {
  double x0 = std::max(1.0, 0.5 * k), delta;
  do
    x0 -= delta = 0.5 * (x0 * x0 - k) / x0;
  while (std::abs(delta / x0) > std::numeric_limits<double>::epsilon());
  return x0;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"x"};
  return GenericTestMain(args, "real_square_root.cc", "real_square_root.tsv",
                         &SquareRoot, DefaultComparator{}, param_names);
}
