#include <string>
#include <unordered_set>
#include "test_framework/generic_test.h"
using std::string;
using std::unordered_set;

// Uses BFS to find the least steps of transformation.
int TransformString(unordered_set<string> D, const string &s, const string &t) {
  if (!D.count(t) || !D.erase(s))
    return -1;
  std::deque<std::pair<string, int>> q{{s, 0}};
  do {
    string &lookup = q.front().first;
    int len = q.front().second;
    if (lookup == t)
      return len;
    for (auto &c : lookup) {
      char orig = c;
      for (c = 'a'; c <= 'z'; ++c)
        if (D.erase(lookup))
          q.emplace_back(lookup, len + 1);
      c = orig;
    }
    q.pop_front();
  } while (!q.empty());
  return -1;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"D", "s", "t"};
  return GenericTestMain(args, "string_transformability.cc",
                         "string_transformability.tsv", &TransformString,
                         DefaultComparator{}, param_names);
}
