#include <string>
#include "test_framework/generic_test.h"
using std::string;

bool CanFormPalindrome(const string &s) {
  std::bitset<128> tbl{};
  for (const auto c: s)
    tbl.flip(c);
  return tbl.count() < 2;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"s"};
  return GenericTestMain(args, "is_string_permutable_to_palindrome.cc",
                         "is_string_permutable_to_palindrome.tsv",
                         &CanFormPalindrome, DefaultComparator{}, param_names);
}
