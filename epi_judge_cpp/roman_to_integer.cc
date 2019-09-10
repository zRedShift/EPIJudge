#include <string>
#include "test_framework/generic_test.h"
using std::string;
using LookupTable = std::array<int, std::numeric_limits<char>::max()>;
constexpr LookupTable GenerateLookupTable() {
  LookupTable tbl{};
  tbl['I'] = 1, tbl['V'] = 5, tbl['X'] = 10, tbl['L'] = 50, tbl['C'] = 100, tbl['D'] = 500, tbl['M'] = 1000;
  return tbl;
}

int RomanToInteger(const string &s) {
  constexpr auto tbl = GenerateLookupTable();
  int num = 0, prev = 0;
  for (const auto c: s) {
    int curr = tbl[c];
    curr > prev ? num -= prev : num += prev;
    prev = curr;
  }
  return num + prev;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"s"};
  return GenericTestMain(args, "roman_to_integer.cc", "roman_to_integer.tsv",
                         &RomanToInteger, DefaultComparator{}, param_names);
}
