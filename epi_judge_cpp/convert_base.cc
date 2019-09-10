#include <string>
#include "test_framework/generic_test.h"
using std::string;

char int_to_char(unsigned int x) {
  return static_cast<char>(x > 9 ? x + 'A' - 10 : x + '0');
}

int char_to_int(char c) {
  return c > '9' ? c - 'A' + 10 : c - '0';
}

string int_to_string(unsigned int x, int b, bool neg) {
  string s;
  do {
    s += int_to_char(x % b);
    x /= b;
  } while (x);
  if (neg)
    s += '-';
  std::reverse(begin(s), end(s));
  return s;
}

unsigned int string_to_int(const string &s, unsigned b, bool neg) {
  return accumulate(begin(s) + neg, end(s), 0, [b](int sum, char c) { return sum * b + char_to_int(c); });
}

string ConvertBase(const string &num_as_string, int b1, int b2) {
  bool neg = num_as_string[0] == '-';
  return int_to_string(string_to_int(num_as_string, b1, neg), b2, neg);
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"num_as_string", "b1", "b2"};
  return GenericTestMain(args, "convert_base.cc", "convert_base.tsv",
                         &ConvertBase, DefaultComparator{}, param_names);
}
