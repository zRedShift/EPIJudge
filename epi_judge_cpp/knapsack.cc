#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
using std::vector;

struct Item {
  int weight, value;
};

int OptimumSubjectToCapacity(const vector<Item> &items, int capacity) {
  vector<int> values(capacity + 1);
  for (const auto[weight, value] : items)
    for (int i = capacity - weight, j = capacity; i >= 0; --i, --j)
      values[j] = std::max(values[j], values[i] + value);
  return values.back();
}

template<>
struct SerializationTraits<Item> : UserSerTraits<Item, int, int> {};

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"items", "capacity"};
  return GenericTestMain(args, "knapsack.cc", "knapsack.tsv",
                         &OptimumSubjectToCapacity, DefaultComparator{},
                         param_names);
}
