#include <string>
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
using std::reverse;
using std::string;
string Decoding(const string &s) {
  string result;
  int n = 0;
  for (const auto c: s)
    if (c > '9') {
      result.append(n, c);
      n = 0;
    } else
      n = n * 10 + c - '0';
  return result;
}
string Encoding(const string &s) {
  string result;
  if (s.empty())
    return result;
  int n = 1;
  char c = s.back();
  for (auto p = s.rbegin() + 1; p != s.rend(); ++p) {
    if (*p != c) {
      result += c;
      c = *p;
      do
        result += '0' + n % 10;
      while (n /= 10);
    }
    ++n;
  }
  result += c;
  do
    result += '0' + n % 10;
  while (n /= 10);
  reverse(result.begin(), result.end());
  return result;
}
void RleTester(const string &encoded, const string &decoded) {
  if (Decoding(encoded) != decoded) {
    throw TestFailure("Decoding failed");
  }
  if (Encoding(decoded) != encoded) {
    throw TestFailure("Encoding failed");
  }
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"encoded", "decoded"};
  return GenericTestMain(args, "run_length_compression.cc",
                         "run_length_compression.tsv", &RleTester,
                         DefaultComparator{}, param_names);
}
