#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

vector<double> OnlineMedian(vector<int>::const_iterator sequence_begin,
                            const vector<int>::const_iterator &sequence_end) {
  if (sequence_begin == sequence_end)
    return {};
  vector<double> median;
  bool odd = true;
  std::priority_queue<int> max_heap;
  std::priority_queue<int, vector<int>, std::greater<>> min_heap;
  max_heap.emplace(*sequence_begin++);
  median.emplace_back(max_heap.top());
  while (sequence_begin != sequence_end) {
    int x = *sequence_begin++;
    if ((odd = !odd)) {
      if (x <= min_heap.top())
        max_heap.emplace(x);
      else {
        max_heap.emplace(min_heap.top());
        min_heap.pop();
        min_heap.emplace(x);
      }
      median.emplace_back(max_heap.top());
    } else {
      if (x >= max_heap.top())
        min_heap.emplace(x);
      else {
        min_heap.emplace(max_heap.top());
        max_heap.pop();
        max_heap.emplace(x);
      }
      median.emplace_back(0.5 * (max_heap.top() + min_heap.top()));
    }
  }
  return median;
}
vector<double> OnlineMedianWrapper(const vector<int> &sequence) {
  return OnlineMedian(cbegin(sequence), cend(sequence));
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"sequence"};
  return GenericTestMain(args, "online_median.cc", "online_median.tsv",
                         &OnlineMedianWrapper, DefaultComparator{},
                         param_names);
}
