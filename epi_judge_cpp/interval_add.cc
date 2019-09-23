#include <vector>
#include "test_framework/fmt_print.h"
#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
using std::vector;

struct Interval {
  int left, right;
};

vector<Interval> AddInterval(const vector<Interval> &disjoint_intervals,
                             Interval new_interval) {
  auto intersection = std::upper_bound(disjoint_intervals.begin(),
                                       disjoint_intervals.end(),
                                       new_interval,
                                       [](const Interval &value, const Interval &element) {
                                         return value.left <= element.right;
                                       });
  vector<Interval> result(disjoint_intervals.begin(), intersection);
  if (intersection != disjoint_intervals.end()) {
    new_interval.left = std::min(new_interval.left, intersection->left);
    intersection = std::upper_bound(intersection,
                                    disjoint_intervals.end(),
                                    new_interval,
                                    [](const Interval &value, const Interval &element) {
                                      return value.right < element.left;
                                    });
    if (intersection != disjoint_intervals.begin())
      new_interval.right = std::max(new_interval.right, std::prev(intersection)->right);
  }
  result.emplace_back(new_interval);
  result.insert(result.end(), intersection, disjoint_intervals.end());
  return result;
}
template<>
struct SerializationTraits<Interval> : UserSerTraits<Interval, int, int> {
  static std::vector<std::string> GetMetricNames(const std::string &arg_name) {
    return {FmtStr("length({})", arg_name)};
  }

  static std::vector<int> GetMetrics(const Interval &x) {
    return {x.right - x.left};
  }
};

bool operator==(const Interval &a, const Interval &b) {
  return a.left == b.left && a.right == b.right;
}

std::ostream &operator<<(std::ostream &out, const Interval &i) {
  return PrintTo(out, std::make_tuple(i.left, i.right));
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"disjoint_intervals", "new_interval"};
  return GenericTestMain(args, "interval_add.cc", "interval_add.tsv",
                         &AddInterval, DefaultComparator{}, param_names);
}
