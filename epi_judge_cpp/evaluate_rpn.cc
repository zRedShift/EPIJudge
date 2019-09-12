#include <string>
#include "test_framework/generic_test.h"
using std::string;
using std::stack;

void Operation(stack<int> &s, char op) {
  int x = s.top();
  s.pop();
  switch (op) {
    case '+': s.top() += x;
      break;
    case '-': s.top() -= x;
      break;
    case '*': s.top() *= x;
      break;
    default: s.top() /= x;
  }
}

int Atoi(const string &s, size_t &i) {
  int x = 0;
  while (s[i] >= '0') {
    x = x * 10 + s[i] - '0';
    ++i;
  }
  return x;
}

int Evaluate(const string &s) {
  stack<int> stack;
  for (size_t i = 0; i < s.size(); ++i)
    if (s[i] >= '0')
      stack.emplace(Atoi(s, i));
    else if (s[i + 1] >= '0')
      stack.emplace(-Atoi(s, ++i));
    else
      Operation(stack, s[i++]);
  return stack.top();
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"expression"};
  return GenericTestMain(args, "evaluate_rpn.cc", "evaluate_rpn.tsv", &Evaluate,
                         DefaultComparator{}, param_names);
}
