#include <vector>
#include "test_framework/generic_test.h"
using std::vector;
unsigned BitPosition(unsigned bit) {
  static constexpr std::array<unsigned, 32> de_bruijn
      {0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8, 31,
       27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9};
  return de_bruijn[bit * 0x077CB531u >> 27u];
}

unsigned BitCount(unsigned bits) {
  unsigned c = ((bits & 0xfffu) * 0x1001001001001ULL & 0x84210842108421ULL) % 0x1f;
  c += (((bits & 0xfff000u) >> 12u) * 0x1001001001001ULL & 0x84210842108421ULL) % 0x1f;
  c += ((bits >> 24u) * 0x1001001001001ULL & 0x84210842108421ULL) % 0x1f;
  return c;
}

vector<vector<int>> GeneratePowerSet(const vector<int> &input_set) {
  unsigned size = 1u << input_set.size();
  vector<vector<int>> result(size);
  while (size--) {
    result[size].reserve(BitCount(size));
    auto output = std::back_inserter(result[size]);
    unsigned bits = size;
    while (bits) {
      unsigned lowest = bits & ~(bits - 1);
      output = input_set[BitPosition(lowest)];
      bits ^= lowest;
    }
  }
  return result;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"input_set"};
  return GenericTestMain(
      args, "power_set.cc", "power_set.tsv", &GeneratePowerSet,
      &UnorderedComparator<vector<vector<int>>>, param_names);
}
