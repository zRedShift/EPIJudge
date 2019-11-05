#include <vector>
#include "test_framework/generic_test.h"
using std::vector;
using std::array;

int FindElementAppearsOnce(const vector<int> &A) {
  array<unsigned, 8 * sizeof(int)> hist{};
  for (unsigned x : A)
    for (int i = 0; x; x >>= 1u)
      hist[i++] += x & 1u;
  unsigned result = 0;
  for (unsigned i = 0; i < hist.size(); ++i)
    result |= (hist[i] % 3u) << i;
  return static_cast<int>(result);
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(args, "element_appearing_once.cc",
                         "element_appearing_once.tsv", &FindElementAppearsOnce,
                         DefaultComparator{}, param_names);
}
