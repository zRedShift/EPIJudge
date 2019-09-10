#include <vector>
#include "test_framework/generic_test.h"
using std::vector;
using std::find_if;
vector<int> PlusOne(vector<int> A) {
  if (find_if(A.rbegin(), A.rend(), [&A](auto &x) { return ++x %= 10; }) == A.rend()) {
    ++A[0];
    A.emplace_back(0);
  }
  return A;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(args, "int_as_array_increment.cc",
                         "int_as_array_increment.tsv", &PlusOne,
                         DefaultComparator{}, param_names);
}
