#include "test_framework/generic_test.h"

constexpr auto tbl = [] {
  std::array<int, 256> tbl{};
  tbl[0] = -1;
  for (auto i = 2; i < tbl.size(); ++i) {
    tbl[i] = tbl[i / 2] + 1;
  }
  return tbl;
}();

int HighestSetBit(int x) {
  if (int tmp = x >> 24)
    return 24 + tbl[tmp];
  else if ((tmp = x >> 16))
    return 16 + tbl[tmp];
  else if ((tmp = x >> 8))
    return 8 + tbl[tmp];
  return tbl[x];
}

int Divide(int x, int y) {
  const int offset = HighestSetBit(y) + 1;
  int q = 0, min = HighestSetBit(x) - offset;
  while (min > -1) {
    q += 1 << min, x -= y << min;
    min = HighestSetBit(x) - offset;
  }
  return (x < y) ? q : q + 1;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"x", "y"};
  return GenericTestMain(args, "primitive_divide.cc", "primitive_divide.tsv",
                         &Divide, DefaultComparator{}, param_names);
}
