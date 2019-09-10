#include <string>
#include "test_framework/generic_test.h"
using LookupTable = std::array<char, std::numeric_limits<char>::max()>;
using std::string;

constexpr LookupTable GenerateLookupTable() {
  LookupTable tbl{0};
  for (char c = 0; c < static_cast<char>(size(tbl)); ++c)
    if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z'))
      tbl[c] = c;
    else if ((c >= 'A' && c <= 'Z'))
      tbl[c] = c + 'a' - 'A';
  return tbl;
}

bool IsPalindrome(const string &s) {
  constexpr auto norm = GenerateLookupTable();
  int i = 0, j = static_cast<int>(size(s) - 1);
  while (i < j) {
    char a, b;
    do {
      a = norm[s[i++]];
    } while (!a);
    do {
      b = norm[s[j--]];
    } while (!b);
    if (a != b)
      return false;
  }
  return true;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"s"};
  return GenericTestMain(args, "is_string_palindromic_punctuation.cc",
                         "is_string_palindromic_punctuation.tsv", &IsPalindrome,
                         DefaultComparator{}, param_names);
}
