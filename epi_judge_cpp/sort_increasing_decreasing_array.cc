#include <vector>
#include "test_framework/generic_test.h"
using std::find_if;
using std::pair;
using std::priority_queue;
using std::make_pair;
using std::vector;
using Iter = vector<int>::const_iterator;
vector<int> SortKIncreasingDecreasingArray(const vector<int> &A) {
  vector<int> result;
  vector<pair<Iter, Iter>> indices;
  auto cmp = [](const pair<int, int> &a, const pair<int, int> &b) { return a.first > b.first; };
  priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(cmp)> min_heap(cmp);
  for (auto p = A.begin(); p != A.end();) {
    auto temp = *p;
    auto end = find_if(p, A.end(), [&temp](int x) {
      if (x < temp)
        return true;
      else {
        temp = x;
        return false;
      }
    });
    min_heap.emplace(make_pair(*p, indices.size()));
    indices.emplace_back(make_pair(p + 1, end));
    if (end == A.end())
      break;
    p = find_if(end, A.end(), [&temp](int x) {
      if (x > temp)
        return true;
      else {
        temp = x;
        return false;
      }
    });
    min_heap.emplace(make_pair(*(p - 1), indices.size()));
    indices.emplace_back(make_pair(p - 1, end));
  }
  while (!min_heap.empty()) {
    auto[x, i] = min_heap.top();
    min_heap.pop();
    result.push_back(x);
    auto &iterator = indices[i];
    if (iterator.first < iterator.second)
      min_heap.emplace(make_pair(*iterator.first++, i));
    else if (iterator.first > iterator.second)
      min_heap.emplace(make_pair(*--iterator.first, i));
  }
  return result;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(args, "sort_increasing_decreasing_array.cc",
                         "sort_increasing_decreasing_array.tsv",
                         &SortKIncreasingDecreasingArray, DefaultComparator{},
                         param_names);
}
