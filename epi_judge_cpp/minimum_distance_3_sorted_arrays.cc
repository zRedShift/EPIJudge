#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

int FindClosestElementsInSortedArrays(const vector<vector<int>> &sorted_arrays) {
  std::multimap<int, std::pair<vector<int>::const_iterator, vector<int>::const_iterator>> range;
  for (const auto &v: sorted_arrays)
    range.emplace(v.front(), std::make_pair(v.cbegin(), v.cend()));
  for (int min = std::numeric_limits<int>::max();;) {
    auto smallest = range.cbegin();
    min = std::min(min, range.crbegin()->first - smallest->first);
    auto iter_pair = smallest->second;
    if (++iter_pair.first == iter_pair.second)
      return min;
    range.erase(smallest);
    range.emplace(*iter_pair.first, iter_pair);
  }
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"sorted_arrays"};
  return GenericTestMain(args, "minimum_distance_3_sorted_arrays.cc",
                         "minimum_distance_3_sorted_arrays.tsv",
                         &FindClosestElementsInSortedArrays,
                         DefaultComparator{}, param_names);
}
