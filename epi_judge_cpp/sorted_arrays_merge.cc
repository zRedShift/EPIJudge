#include <vector>
#include "test_framework/generic_test.h"
using std::pair;
using std::priority_queue;
using std::vector;
vector<int> MergeSortedArrays(const vector<vector<int>> &sorted_arrays) {
  vector<int> result, indices(sorted_arrays.size(), 1);
  auto cmp = [](const pair<int, int> &a, const pair<int, int> &b) { return a.first > b.first; };
  priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(cmp)> min_heap(cmp);
  for (int i = 0; i < sorted_arrays.size(); ++i)
    min_heap.emplace(pair<int, int>{sorted_arrays[i][0], i});
  while (!min_heap.empty()) {
    auto[x, i] = min_heap.top();
    min_heap.pop();
    result.push_back(x);
    if (indices[i] < sorted_arrays[i].size())
      min_heap.emplace(pair<int, int>{sorted_arrays[i][indices[i]++], i});
  }
  return result;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"sorted_arrays"};
  return GenericTestMain(args, "sorted_arrays_merge.cc",
                         "sorted_arrays_merge.tsv", &MergeSortedArrays,
                         DefaultComparator{}, param_names);
}
