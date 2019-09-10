#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/timed_executor.h"
using std::for_each;
using std::swap;
using std::vector;
// Returns the number of valid entries after deletion.
int DeleteDuplicates(vector<int> *A_ptr) {
  auto &vec = *A_ptr;
  int n = size(vec), write_index = 0;
  if (n < 2) {
    return n;
  }
  for (int i = 1; i != n; ++i) {
    if (vec[i] != vec[write_index]) {
      vec[++write_index] = vec[i];
    }
  }
  return write_index + 1;
}
vector<int> DeleteDuplicatesWrapper(TimedExecutor &executor, vector<int> A) {
  int end = executor.Run([&] { return DeleteDuplicates(&A); });
  A.resize(end);
  return A;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "A"};
  return GenericTestMain(
      args, "sorted_array_remove_dups.cc", "sorted_array_remove_dups.tsv",
      &DeleteDuplicatesWrapper, DefaultComparator{}, param_names);
}
