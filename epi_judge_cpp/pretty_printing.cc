#include <string>
#include <vector>
#include "test_framework/generic_test.h"
using std::string;
using std::vector;

int MinimumMessiness(const vector<string> &words, int line_length) {
  vector<int> minimum_messiness(words.size(), std::numeric_limits<int>::max());
  int i = words.size(), remaining = line_length + 1;
  while (i-- && (remaining -= 1 + words[i].size()) >= 0)
    minimum_messiness[i] = remaining * remaining;
  for (remaining = line_length + 1; i >= 0; --i, remaining = line_length + 1)
    for (int j = i; (remaining -= 1 + words[j++].size()) >= 0;)
      minimum_messiness[i] = std::min(minimum_messiness[i], remaining * remaining + minimum_messiness[j]);
  return minimum_messiness.front();
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"words", "line_length"};
  return GenericTestMain(args, "pretty_printing.cc", "pretty_printing.tsv",
                         &MinimumMessiness, DefaultComparator{}, param_names);
}
