#include <string>
#include "test_framework/generic_test.h"
using std::string;
bool IsWellFormed(const string &s) {
  std::stack<char> stack;
  for (const auto c: s) {
    switch (c) {
      case '(':
      case '{':
      case '[':stack.emplace(c);
        continue;
      default:
        if (stack.empty() || abs(c - stack.top()) > 2)
          return false;
        stack.pop();
    }
  }
  return stack.empty();
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"s"};
  return GenericTestMain(args, "is_valid_parenthesization.cc",
                         "is_valid_parenthesization.tsv", &IsWellFormed,
                         DefaultComparator{}, param_names);
}
