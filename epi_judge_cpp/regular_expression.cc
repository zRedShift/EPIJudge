#include <string>
#include "test_framework/generic_test.h"
using std::string;

bool IsMatchHelper(const char *regex, const char *s) {
  const char r = *regex++, c = *s;
  switch (r) {
    case 0:return true;
    case '$':return !c;
    case '.':
      if (*regex != '*')
        return c && IsMatchHelper(regex, s + 1);
      ++regex;
      do {
        if (IsMatchHelper(regex, s))
          return true;
      } while (*s++);
      return false;
    default:
      if (*regex != '*')
        return r == c && IsMatchHelper(regex, s + 1);
      ++regex;
      do {
        if (IsMatchHelper(regex, s))
          return true;
      } while (*s++ == r);
      return false;
  }
}

bool IsMatch(const string &regex, const string &s) {
  if (regex.empty())
    return true;
  if (regex.front() == '^')
    return IsMatchHelper(regex.c_str() + 1, s.c_str());
  for (auto iter = s.c_str(); *iter; ++iter)
    if (IsMatchHelper(regex.c_str(), iter))
      return true;
  return IsMatchHelper(regex.c_str(), "");
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"regex", "s"};
  return GenericTestMain(args, "regular_expression.cc",
                         "regular_expression.tsv", &IsMatch,
                         DefaultComparator{}, param_names);
}
