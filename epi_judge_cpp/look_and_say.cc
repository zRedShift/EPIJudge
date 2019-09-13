#include <string>
#include "test_framework/generic_test.h"
using std::string;

string LookAndSay(int n) {
  string prev, next = "1";
  for (int i = 1; i < n; ++i) {
    prev.swap(next);
    next.clear();
    int d;
    for (auto curr = begin(prev); curr != end(prev); curr += d) {
      d = distance(curr, find_if(curr + 1, end(prev), [&curr](char c) { return *curr != c; }));
      next.push_back(d + '0');
      next.push_back(*curr);
    }
  }
  return next;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"n"};
  return GenericTestMain(args, "look_and_say.cc", "look_and_say.tsv",
                         &LookAndSay, DefaultComparator{}, param_names);
}
