#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
using std::vector;

struct Interval {
  int left, right;
  bool operator<(const Interval &rhs) const {
    return right < rhs.right;
  }
};

int FindMinimumVisits(vector<Interval> intervals) {
  std::sort(intervals.begin(), intervals.end());
  int result = 0, curr = std::numeric_limits<int>::min();
  for (const auto[left, right] : intervals)
    if (left > curr)
      ++result, curr = right;
  return result;
}
template<>
struct SerializationTraits<Interval> : UserSerTraits<Interval, int, int> {};

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"intervals"};
  return GenericTestMain(args, "minimum_points_covering_intervals.cc",
                         "minimum_points_covering_intervals.tsv",
                         &FindMinimumVisits, DefaultComparator{}, param_names);
}
