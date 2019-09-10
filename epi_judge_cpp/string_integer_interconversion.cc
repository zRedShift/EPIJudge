#include <string>
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
using std::accumulate;
using std::make_pair;
using std::string;

string IntToString(int x) {
  string s;
  unsigned y = x;
  bool neg = false;
  if (x < 0) {
    neg = true;
    y = -x;
  }
  do {
    s += static_cast<char>('0' + y % 10);
    y /= 10;
  } while (y);
  if (neg)
    s += '-';
  std::reverse(begin(s), end(s));
  return s;
}
int StringToInt(const string &s) {
  int sign = 1;
  auto front = begin(s);
  if (*front == '-') {
    sign = -1;
    ++front;
  }
  return sign * accumulate(front, end(s), 0, [](int sum, char c) { return sum * 10 + c - '0'; });
}
void Wrapper(int x, const string &s) {
  if (IntToString(x) != s) {
    throw TestFailure("Int to string conversion failed");
  }

  if (StringToInt(s) != x) {
    throw TestFailure("String to int conversion failed");
  }
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"x", "s"};
  return GenericTestMain(args, "string_integer_interconversion.cc",
                         "string_integer_interconversion.tsv", &Wrapper,
                         DefaultComparator{}, param_names);
}
