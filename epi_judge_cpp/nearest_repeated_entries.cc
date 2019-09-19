#include <string>
#include <vector>
#include "test_framework/generic_test.h"
using std::string;
using std::vector;

int FindNearestRepetition(const vector<string> &paragraph) {
  std::unordered_map<string, int> last_position;
  int distance = std::numeric_limits<int>::max();
  for (int i = 0; i < paragraph.size(); ++i)
    if (auto prev = last_position.emplace(paragraph[i], i); !prev.second)
      distance = std::min(distance, i - prev.first->second), prev.first->second = i;
  return distance == std::numeric_limits<int>::max() ? -1 : distance;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"paragraph"};
  return GenericTestMain(args, "nearest_repeated_entries.cc",
                         "nearest_repeated_entries.tsv", &FindNearestRepetition,
                         DefaultComparator{}, param_names);
}
