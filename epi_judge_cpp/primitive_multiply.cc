#include "test_framework/generic_test.h"
unsigned long long Add(unsigned long long x, unsigned long long y) {
  while (y) {
    auto and_result = x & y;
    x ^= y;
    y = and_result << 1u;
  }
  return x;
}

unsigned long long Multiply(unsigned long long x, unsigned long long y) {
  auto result = 0ull;
  while (y) {
    if (y & 1u)
      result = Add(result, x);
    x <<= 1u, y >>= 1u;
  }
  return result;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"x", "y"};
  return GenericTestMain(args, "primitive_multiply.cc",
                         "primitive_multiply.tsv", &Multiply,
                         DefaultComparator{}, param_names);
}
