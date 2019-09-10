#include <vector>
#include "test_framework/generic_test.h"
using std::vector;
void check_sign(int &num, int &sign) {
  if (num < 0) {
    num = -num;
    sign = -sign;
  }
}

vector<int> Multiply(vector<int> num1, vector<int> num2) {
  // assuming no empty vectors
  if (!num1.front() || !num2.front())
    return {0};
  int sign = 1, jump = static_cast<int>(size(num2)) - 1;
  check_sign(num1.front(), sign);
  check_sign(num2.front(), sign);
  vector<int> answer(size(num1) + jump + ((num1.front() + 1) * (num2.front() + 1) > 9));
  auto z = rbegin(answer);
  for (auto x = rbegin(num1); x != rend(num1); ++x) {
    for (auto y = rbegin(num2); y != rend(num2); ++y) {
      int carry = *z + *x * *y;
      *z = carry % 10;
      *++z += carry / 10;
    }
    z -= jump;
  }
  if (!answer.front())
    answer = {begin(answer) + 1, end(answer)};
  answer.front() *= sign;
  return answer;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"num1", "num2"};
  return GenericTestMain(args, "int_as_array_multiply.cc",
                         "int_as_array_multiply.tsv", &Multiply,
                         DefaultComparator{}, param_names);
}
