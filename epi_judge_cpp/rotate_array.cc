#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/timed_executor.h"
using std::vector;

void RotateArray(int rotate_amount, vector<int> *A_ptr) {
  auto &A = *A_ptr;
  int actual_amount = rotate_amount % A.size(), gcd = std::gcd(actual_amount, A.size());
  if (!actual_amount)
    return;
  for (int i = 0, num = actual_amount; i < gcd; ++i, ++num)
    do {
      for (; num < A.size(); num += actual_amount)
        std::swap(A[i], A[num]);
    } while ((num %= A.size()) != i);
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
