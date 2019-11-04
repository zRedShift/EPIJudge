#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
using std::deque;
using std::vector;

struct TrafficElement {
  bool operator==(const TrafficElement &that) const {
    return time == that.time && volume == that.volume;
  }
  int time;
  double volume;
};
vector<TrafficElement> CalculateTrafficVolumes(const vector<TrafficElement> &A, int w) {
  deque<TrafficElement> volumes;
  vector<TrafficElement> result(A);
  deque<double> max_;
  for (auto &e : result) {
    int last = e.time - w;
    bool max_clear = max_.empty() || max_.front() < e.volume, vol_clear = volumes.empty() || volumes.back().time < last;
    if ((max_clear |= vol_clear))
      max_.clear();
    if (vol_clear)
      volumes.clear();
    if (!max_clear)
      while (max_.back() < e.volume)
        max_.pop_back();
    max_.emplace_back(e.volume);
    if (!vol_clear)
      while (volumes.front().time < last) {
        if (volumes.front().volume == max_.front())
          max_.pop_front();
        volumes.pop_front();
      }
    volumes.emplace_back(e);
    e.volume = max_.front();
  }
  return result;
}
template<>
struct SerializationTraits<TrafficElement>
    : UserSerTraits<TrafficElement, int, double> {
};

std::ostream &operator<<(std::ostream &out, const TrafficElement &te) {
  return out << '[' << te.time << ", " << te.volume << ']';
};

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A", "w"};
  return GenericTestMain(args, "max_of_sliding_window.cc",
                         "max_of_sliding_window.tsv", &CalculateTrafficVolumes,
                         DefaultComparator{}, param_names);
}
