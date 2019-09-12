#include <iterator>
#include <vector>
#include "test_framework/generic_test.h"
using std::pair;
using std::vector;
vector<int> ExamineBuildingsWithSunset(
    vector<int>::const_iterator sequence_begin,
    const vector<int>::const_iterator &sequence_end) {
  vector<pair<int, int>> heights;
  for (int i = 0; sequence_begin != sequence_end; ++i, ++sequence_begin) {
    while (!heights.empty() && *sequence_begin >= heights.back().second)
      heights.pop_back();
    heights.emplace_back(pair{i, *sequence_begin});
  }
  vector<int> sunny;
  sunny.reserve(heights.size());
  for_each(heights.rbegin(), heights.rend(), [&sunny](auto p) { sunny.emplace_back(p.first); });
  return sunny;
}

vector<int> ExamineBuildingsWithSunsetReverse(
    vector<int>::const_iterator sequence_begin,
    const vector<int>::const_iterator &sequence_end) {
  vector<int> sunny;
  int i = static_cast<int>(std::distance(sequence_begin, sequence_end) - 1), max = std::numeric_limits<int>::min();
  for (auto p = sequence_end - 1; p != sequence_begin - 1; --p, --i)
    if (*p > max)
      sunny.emplace_back(i), max = *p;
  return sunny;
}

vector<int> ExamineBuildingsWithSunsetWrapper(const vector<int> &sequence) {
  return ExamineBuildingsWithSunset(cbegin(sequence), cend(sequence));
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"sequence"};
  return GenericTestMain(args, "sunset_view.cc", "sunset_view.tsv",
                         &ExamineBuildingsWithSunsetWrapper,
                         DefaultComparator{}, param_names);
}
