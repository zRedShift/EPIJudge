#include <variant>
#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
using std::vector;

struct Event {
  int start, finish;
};
struct Delimiter {
  bool operator<(const Delimiter &rhs) const {
    return value == rhs.value ? !is_end && rhs.is_end : value < rhs.value;
  }
  int value;
  bool is_end;
};
int FindMaxSimultaneousEvents(const vector<Event>& A) {
  vector<Delimiter> vec;
  for (const auto event: A) {
    vec.emplace_back(Delimiter{event.start, false});
    vec.emplace_back(Delimiter{event.finish, true});
  }
  std::sort(vec.begin(), vec.end());
  int simultaneous = 0, simultaneous_max = 0;
  for (const auto[_, is_end]: vec)
    if (is_end)
      simultaneous_max = std::max(simultaneous_max, simultaneous), --simultaneous;
    else
      ++simultaneous;
  return simultaneous_max;
}
template <>
struct SerializationTraits<Event> : UserSerTraits<Event, int, int> {};

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(args, "calendar_rendering.cc",
                         "calendar_rendering.tsv", &FindMaxSimultaneousEvents,
                         DefaultComparator{}, param_names);
}
