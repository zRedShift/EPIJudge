#include <string>
#include <vector>
#include "test_framework/generic_test.h"
using std::string;
using std::vector;

void RecursiveFill(const string &p, int i, string *s, vector<string> &v) {
  const auto tbl = "0\0\0\0\0001\0\0\0\0ABC\0\0DEF\0\0GHI\0\0JKL\0\0MNO\0\0PQRS\0TUV\0\0WXYZ";
  if (size(p) == i)
    v.emplace_back(*s);
  else {
    auto x = &tbl[(p[i] - '0') * 5];
    while (*x) {
      (*s)[i] = *x++;
      RecursiveFill(p, i + 1, s, v);
    }
  }
}

vector<string> PhoneMnemonic(const string &phone_number) {
  vector<string> comb;
  string s(size(phone_number), 0);
  RecursiveFill(phone_number, 0, &s, comb);
  return comb;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"phone_number"};
  return GenericTestMain(args, "phone_number_mnemonic.cc",
                         "phone_number_mnemonic.tsv", &PhoneMnemonic,
                         &UnorderedComparator<std::vector<std::string>>,
                         param_names);
}
