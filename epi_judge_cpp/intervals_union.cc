#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include "test_framework/timed_executor.h"
using std::vector;

struct Interval {
  struct Endpoint {
    bool is_closed;
    int val;
  };
  bool operator<(const Interval &rhs) const {
    return left.val != rhs.left.val ? left.val < rhs.left.val : left.is_closed && !rhs.left.is_closed;
  }
  Endpoint left, right;
};

vector<Interval> UnionOfIntervals(vector<Interval> intervals) {
  if (intervals.empty())
    return {};
  std::sort(intervals.begin(), intervals.end());
  vector<Interval> result;
  auto curr(intervals.front());
  for (const auto &interval: intervals)
    if (interval.left.val > curr.right.val
        || (interval.left.val == curr.right.val && !interval.left.is_closed && !curr.right.is_closed))
      result.emplace_back(curr), curr = interval;
    else if (interval.right.val > curr.right.val || (interval.right.val == curr.right.val && interval.right.is_closed))
      curr.right = interval.right;
  result.emplace_back(curr);
  return result;
}
struct FlatInterval {
  int left_val;
  bool left_is_closed;
  int right_val;
  bool right_is_closed;

  FlatInterval(int left_val, bool left_is_closed, int right_val,
               bool right_is_closed)
      : left_val(left_val),
        left_is_closed(left_is_closed),
        right_val(right_val),
        right_is_closed(right_is_closed) {}

  explicit FlatInterval(Interval in)
      : left_val(in.left.val),
        left_is_closed(in.left.is_closed),
        right_val(in.right.val),
        right_is_closed(in.right.is_closed) {}

  operator Interval() const {
    return {{left_is_closed, left_val}, {right_is_closed, right_val}};
  }

  bool operator==(const FlatInterval &rhs) const {
    return std::tie(left_val, left_is_closed, right_val, right_is_closed) ==
        std::tie(rhs.left_val, rhs.left_is_closed, rhs.right_val,
                 rhs.right_is_closed);
  }
};

template<>
struct SerializationTraits<FlatInterval>
    : UserSerTraits<FlatInterval, int, bool, int, bool> {
};

std::ostream &operator<<(std::ostream &out, const FlatInterval &i) {
  return out << (i.left_is_closed ? '<' : '(') << i.left_val << ", "
             << i.right_val << (i.right_is_closed ? '>' : ')');
}

std::vector<FlatInterval> UnionOfIntervalsWrapper(
    TimedExecutor &executor, const std::vector<FlatInterval> &intervals) {
  std::vector<Interval> casted;
  for (const FlatInterval &i : intervals) {
    casted.push_back(static_cast<Interval>(i));
  }

  std::vector<Interval> result =
      executor.Run([&] { return UnionOfIntervals(casted); });

  return {begin(result), end(result)};
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "intervals"};
  return GenericTestMain(args, "intervals_union.cc", "intervals_union.tsv",
                         &UnionOfIntervalsWrapper, DefaultComparator{},
                         param_names);
}
