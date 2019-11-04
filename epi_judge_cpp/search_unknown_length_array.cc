#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

int BinarySearchUnknownLength(const vector<int> &A, int k) {
  int right = 0;
  for (; right != -1; right = (right << 1) | 1)
    try {
      if (int x = A.at(right); x == k)
        return right;
      else if (x > k)
        break;
    } catch (const std::out_of_range &) {
      break;
    }
  int left = right-- >> 1;
  while (left <= right) {
    int middle = left + (right - left) / 2;
    try {
      if (int x = A.at(middle); x < k)
        left = middle + 1;
      else if (x == k)
        return middle;
      else
        right = middle - 1;
    } catch (const std::out_of_range &) {
      right = middle - 1;
    }
  }
  return -1;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A", "k"};
  return GenericTestMain(
      args, "search_unknown_length_array.cc", "search_unknown_length_array.tsv",
      &BinarySearchUnknownLength, DefaultComparator{}, param_names);
}
