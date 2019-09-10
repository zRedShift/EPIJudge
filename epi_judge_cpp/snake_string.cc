#include <string>
#include "test_framework/generic_test.h"
using std::string;

string SnakeString(const string& s) {
  string ans;
  ans.reserve(s.size());
  for (auto i = 1; i < s.size(); i += 4)
    ans +=  s[i];
  for (auto i = 0; i < s.size(); i += 2)
    ans +=  s[i];
  for (auto i = 3; i < s.size(); i += 4)
    ans +=  s[i];
  return ans;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"s"};
  return GenericTestMain(args, "snake_string.cc", "snake_string.tsv",
                         &SnakeString, DefaultComparator{}, param_names);
}
