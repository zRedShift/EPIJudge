#include <vector>
#include "test_framework/generic_test.h"
using std::vector;
int HIndex(const vector<int> &citations) {
  const int n = citations.size();
  vector<int> histogram(n);
  for (const auto x: citations)
    if (x < n)
      ++histogram[x];
  for (int i = 0, sum = 0; i < n; ++i)
    if ((sum += histogram[i]) >= n - i)
      return i;
  return n;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"citations"};
  return GenericTestMain(args, "h_index.cc", "h_index.tsv", &HIndex,
                         DefaultComparator{}, param_names);
}
