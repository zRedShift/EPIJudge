#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
using std::vector;
struct MinMax {
  int smallest, largest;
};

MinMax FindMinMax(const vector<int> &A) {
  if (A.size() < 1)
    throw std::length_error("can't determine min/max from an empty vector");
  int min = A.front(), max = A.front(), i = 2 - A.size() % 2;
  if (i == 2)
    std::tie(min, max) = std::minmax(A[0], A[1]);
  for (; i < A.size(); i += 2)
    if (A[i] > A[i + 1])
      max = std::max(max, A[i]), min = std::min(min, A[i + 1]);
    else
      max = std::max(max, A[i + 1]), min = std::min(min, A[i]);
  return {min, max};
}
template<>
struct SerializationTraits<MinMax> : UserSerTraits<MinMax, int, int> {};

bool operator==(const MinMax &lhs, const MinMax &rhs) {
  return std::tie(lhs.smallest, lhs.largest) ==
      std::tie(rhs.smallest, rhs.largest);
}

std::ostream &operator<<(std::ostream &out, const MinMax &x) {
  return out << "min: " << x.smallest << ", max: " << x.largest;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(args, "search_for_min_max_in_array.cc",
                         "search_for_min_max_in_array.tsv", &FindMinMax,
                         DefaultComparator{}, param_names);
}
