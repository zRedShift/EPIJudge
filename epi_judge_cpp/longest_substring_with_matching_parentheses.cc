#include <string>
#include <vector>
#include "test_framework/generic_test.h"
using std::string;
using std::vector;

int LongestMatchingParentheses(const string &s) {
  std::vector<int> stack;
  stack.reserve(s.size() / 2);
  int prev = 0, max = 0;
  for (const char c : s)
    if (c == '(')
      stack.emplace_back(0);
    else if (!stack.empty()) {
      int top = stack.back() + 1;
      if (stack.pop_back(); !stack.empty())
        stack.back() += top;
      else
        prev += top;
    } else
      max = std::max(max, prev), prev = 0;
  return 2 * std::max({max, prev, !stack.empty() ? *std::max_element(stack.begin(), stack.end()) : 0});
}

template<char paren, typename IterType>
int Parse(const IterType &begin, const IterType &end) {
  int max = 0, count = 0, curr = 0;
  for (IterType i = begin; i != end; ++i)
    if (*i == paren)
      ++count, ++curr;
    else if (curr) {
      if (--curr, ++count; !curr)
        max = std::max(max, count);
    } else
      curr = count = 0;
  return max;
}

int LongestMatchingParenthesesAlt(const string &s) {
  return std::max(Parse<'('>(s.cbegin(), s.cend()), Parse<')'>(s.crbegin(), s.crend()));
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"s"};
  return GenericTestMain(args, "longest_substring_with_matching_parentheses.cc",
                         "longest_substring_with_matching_parentheses.tsv",
                         &LongestMatchingParentheses, DefaultComparator{},
                         param_names);
}
