#include "test_framework/generic_test.h"

using lookup_table = std::array<unsigned short, 65536>;

constexpr unsigned short reverse(short x) {
  for (char i = 0; i < 8; ++i)
    if ((x >> i & 1) ^ (x >> (15 - i) & 1))
      x ^= (1 << i) | (1 << (15 - i));
  return x;
}

constexpr lookup_table generate_table() {
  lookup_table tbl{};
  for (auto i = 0; i < tbl.size(); ++i) {
    tbl[i] = reverse(i);
  }
  return tbl;
}

constexpr lookup_table tbl = generate_table();
constexpr unsigned long long mask = (1u << 16u) - 1;

unsigned long long ReverseBits(unsigned long long x) {
  return ((unsigned long long) tbl[x & mask] << 48u) + ((unsigned long long) tbl[x >> 16u & mask] << 32u)
      + ((unsigned long long) tbl[x >> 32u & mask] << 16u) + (unsigned long long) tbl[x >> 48u];
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"x"};
  return GenericTestMain(args, "reverse_bits.cc", "reverse_bits.tsv",
                         &ReverseBits, DefaultComparator{}, param_names);
}
