#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

vector<int> IntersectTwoSortedArrays(const vector<int> &A, const vector<int> &B) {
  if (A.empty() || B.empty())
    return {};
  vector<int>::const_iterator a_begin, a_end, b_begin, b_end;
  if (A.front() < B.front())
    a_begin = std::lower_bound(A.begin(), A.end(), B.front()), b_begin = B.begin();
  else
    a_begin = A.begin(), b_begin = std::lower_bound(B.begin(), B.end(), A.front());
  a_end = std::upper_bound(a_begin, A.end(), B.back()), b_end = std::upper_bound(b_begin, B.end(), A.back());
  if (a_begin == a_end || b_begin == b_end)
    return {};
  int a = std::distance(a_begin, a_end), b = std::distance(b_begin, b_end);
  if (a > b)
    std::swap(a_begin, b_begin), std::swap(a_end, b_end), std::swap(a, b);
  vector<int> result;
  static int curr;
  static constexpr auto kPred = [](const int &x) { return curr <= x; };
  const bool kBinary = a * std::ilogb(b) < a + b;
  do {
    curr = *a_begin;
    if ((b_begin = kBinary ? std::lower_bound(b_begin, b_end, *a_begin) : std::find_if(b_begin, b_end, kPred)) == b_end)
      break;
    if (*a_begin == (curr = *b_begin))
      result.emplace_back(curr++), ++b_begin;
  } while ((a_begin = std::find_if(a_begin + 1, a_end, kPred)) != a_end);
  return result;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A", "B"};
  return GenericTestMain(
      args, "intersect_sorted_arrays.cc", "intersect_sorted_arrays.tsv",
      &IntersectTwoSortedArrays, DefaultComparator{}, param_names);
}
