#include "test_framework/generic_test.h"

int SquareRoot(int k) {
  static const int max_sqrt = std::sqrt(std::numeric_limits<int>::max());
  int left = 0, right = std::min((k + 1) / 2, max_sqrt);
  while (left <= right) {
    int mid = left + (right - left) / 2;
    if (mid * mid > k)
      right = mid - 1;
    else
      left = mid + 1;
  }
  return right;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"k"};
  return GenericTestMain(args, "int_square_root.cc", "int_square_root.tsv",
                         &SquareRoot, DefaultComparator{}, param_names);
}
