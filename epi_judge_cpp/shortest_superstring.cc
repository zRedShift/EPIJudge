#include <string>
#include <vector>
#include "test_framework/generic_test.h"
using std::string;
using std::vector;
using std::pair;
using std::make_pair;
struct FirstLastOverlap {
  int first = 0, last = 0, overlap = -1;
  bool operator<(const FirstLastOverlap &rhs) const {
    return overlap < rhs.overlap;
  }
};

FirstLastOverlap FillCacheHelper(const int a, const int b, const int i, const FirstLastOverlap &curr) {
  return a > b ? FirstLastOverlap{i, curr.last, a + curr.overlap} : FirstLastOverlap{curr.first, i, b + curr.overlap};
}

void FillCache(vector<FirstLastOverlap> &cache, const vector<vector<int>> &adjacency, const int bits, const int N) {
  FirstLastOverlap best;
  for (int i = 0; i < N; ++i) {
    int shift = 1 << i;
    if (!(bits & shift))
      continue;
    auto &c = cache[shift = bits ^ shift];
    if (c.overlap == -1)
      FillCache(cache, adjacency, shift, N);
    best = std::max(best, FillCacheHelper(adjacency[i][c.first], adjacency[c.last][i], i, c));
  }
  cache[bits] = best;
}

int Overlap(const string &a, const string &b) {
  for (int i = 1 + std::max(0, int(a.size() - b.size())); i < a.size(); ++i) {
    int count = a.size() - i;
    if (std::string_view(b.data(), count) == a.data() + i)
      return count;
  }
  return 0;
}

int ShortestSuperstring(const vector<string> &A) {
  int N = A.size(), result = 0;
  vector<FirstLastOverlap> cache(1 << N);
  vector<vector<int>> adjacency(N, vector<int>(N));
  for (int i = 0; i < N; ++i) {
    int shift = 1 << i;
    result += A[i].size();
    auto &c = cache[shift] = {i, i, 0};
    for (int j = i + 1; j < N; ++j)
      cache[shift + (1 << j)] =
          FillCacheHelper(adjacency[j][i] = Overlap(A[j], A[i]), adjacency[i][j] = Overlap(A[i], A[j]), j, c);
  }
  if (N > 2)
    FillCache(cache, adjacency, cache.size() - 1, N);
  return result - cache.back().overlap;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"strings"};
  return GenericTestMain(args, "shortest_superstring.cc",
                         "shortest_superstring.tsv", &ShortestSuperstring,
                         DefaultComparator{}, param_names);
}
