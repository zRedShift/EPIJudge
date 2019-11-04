#include <string>
#include <vector>
#include "test_framework/generic_test.h"
using std::string;
using std::vector;

vector<string> JustifyText(const vector<string> &words, int L) {
  vector<string> result;
  for (auto start = words.cbegin(), next = start;;) {
    int curr = -1;
    for (; next != words.cend() && curr + int(next->size()) < L; curr += int(next++->size()) + 1) continue;
    string &line = result.emplace_back();
    line.reserve(L), line = *start++;
    if (next == words.cend()) {
      for (; start != next; ++start)
        line += ' ', line += *start;
      line.resize(L, ' ');
      return result;
    }
    if (int len = std::distance(start, next), remaining = L - curr; len) {
      int space = remaining / len + 2;
      for (auto leftover = start + remaining % len; start != leftover; ++start)
        line.append(space, ' '), line += *start;
      for (--space; start != next; ++start)
        line.append(space, ' '), line += *start;
    } else
      line.resize(L, ' ');
  }
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"words", "L"};
  return GenericTestMain(args, "left_right_justify_text.cc",
                         "left_right_justify_text.tsv", &JustifyText,
                         DefaultComparator{}, param_names);
}
