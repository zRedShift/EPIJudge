#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

vector<int> KLargestInBinaryHeap(const vector<int> &heap, int k) {
  auto cmp = [&heap](const int a, const int b) { return heap[a] < heap[b]; };
  std::priority_queue<int, vector<int>, decltype(cmp)> max_heap(cmp);
  vector<int> result;
  result.reserve(k);
  max_heap.push(0);
  while (k--) {
    int i = max_heap.top();
    max_heap.pop();
    result.push_back(heap[i]);
    if (2 * i + 1 < heap.size())
      max_heap.push(2 * i + 1);
    if (2 * i + 2 < heap.size())
      max_heap.push(2 * i + 2);
  }
  return result;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A", "k"};
  return GenericTestMain(args, "k_largest_in_heap.cc", "k_largest_in_heap.tsv",
                         &KLargestInBinaryHeap,
                         &UnorderedComparator<std::vector<int>>, param_names);
}
