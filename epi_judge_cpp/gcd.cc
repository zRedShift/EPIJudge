#include "test_framework/generic_test.h"

size_t Gcd(size_t x, size_t y) {
  if (!x || !y)
    return std::max(x, y);
  size_t shift = 0;
  while (!((x | y) & 1u))
    x >>= 1u, y >>= 1u, ++shift;
  while (!(x & 1u))
    x >>= 1u;
  while (!(y & 1u))
    y >>= 1u;
  if (x > y)
    std::swap(x, y);
  while (x != y && x != 1) {
    size_t temp = y - x;
    do {
      temp >>= 1u;
    } while (!(temp & 1u));
    std::tie(x, y) = std::minmax({x, temp});
  }
  return x << shift;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"x", "y"};
  return GenericTestMain(args, "gcd.cc", "gcd.tsv", &Gcd, DefaultComparator{},
                         param_names);
}
