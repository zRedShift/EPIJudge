#include "test_framework/generic_test.h"

using lookup_table = std::array<char, 65536>;

constexpr lookup_table generate_table() {
  lookup_table tbl{};
  for (unsigned long i = 0; i < tbl.size(); ++i)
    tbl[i] = (i & 1u) + tbl[i / 2];
  return tbl;
}

constexpr lookup_table tbl = generate_table();
constexpr unsigned long long mask = (1u << 16u) - 1;

short Parity(unsigned long long x) {
  x ^= x >> 32u;
  x ^= x >> 16u;
//  x ^= x >> 8u;
//  x ^= x >> 4u;
//  x ^= x >> 2u;
//  x ^= x >> 1u;
//  return x & 1;
  return tbl[x & mask] % 2;
//  return (tbl[x & mask] + tbl[x >> 16u & mask] + tbl[x >> 32u & mask] + tbl[x >> 48u]) % 2;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"x"};
  return GenericTestMain(args, "parity.cc", "parity.tsv", &Parity,
                         DefaultComparator{}, param_names);
}
