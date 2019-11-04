#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/timed_executor.h"
using std::distance;
using std::vector;
using std::swap;
using std::swap_ranges;
using std::reverse;

void RotateArrayAltAlt(int rotate, vector<int> *A_ptr) {
  auto &A = *A_ptr;
  if (int gcd = std::gcd(rotate %= A.size(), A.size()); rotate)
    for (int i = 0, num = rotate; i < gcd; ++i, ++num)
      do {
        for (; num < A.size(); num += rotate)
          std::swap(A[i], A[num]);
      } while ((num %= A.size()) != i);
}

void RotateArrayAlt(int rotate, vector<int> *A) {
  if (int size = A->size(), complement = size - (rotate %= size); rotate) {
    auto begin = A->begin();
    auto rbegin = A->rbegin();
    while (rotate && size != rotate)
      if (rotate > complement) {
        swap_ranges(begin, begin + rotate, begin + complement), begin += rotate;
        complement -= size % complement, size -= rotate, rotate = size - complement;
      } else {
        swap_ranges(rbegin, rbegin + complement, rbegin + rotate), rbegin += complement;
        rotate -= size % rotate, size -= complement, complement = size - rotate;
      }
  }
}

void RotateArray(int rotate, vector<int> *A) {
  if (rotate %= A->size())
    reverse(begin(*A), end(*A)), reverse(begin(*A), begin(*A) + rotate), reverse(begin(*A) + rotate, end(*A));
}

vector<int> RotateArrayWrapper(TimedExecutor &executor, vector<int> A,
                               int rotate_amount) {
  executor.Run([&] { RotateArray(rotate_amount, &A); });
  return A;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "A", "rotate_amount"};
  return GenericTestMain(args, "rotate_array.cc", "rotate_array.tsv",
                         &RotateArrayWrapper, DefaultComparator{}, param_names);
}
