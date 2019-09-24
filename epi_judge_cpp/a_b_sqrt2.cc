#include <vector>
#include "test_framework/generic_test.h"
using std::vector;
struct ABSqrt2 {
  ABSqrt2(int a, int b) : a(a), b(b), val(sqrt2 * b + a) {}
  bool operator<(const ABSqrt2 &rhs) const {
    return val < rhs.val;
  }
  bool operator==(const ABSqrt2 &rhs) const {
    return val == rhs.val;
  }
  int a, b;
  double val;
  constexpr static double sqrt2 = 1.4142135623730950488;
};

vector<double> GenerateFirstKABSqrt2(int k) {
  vector<ABSqrt2> vec;
  vector<double> result;
  vec.reserve(k), result.reserve(k);
  vec.emplace_back(0, 0), result.emplace_back(0.0);
  int i = 0, j = 0;
  while (--k) {
    ABSqrt2 one(vec[i].a + 1, vec[i].b), two(vec[j].a, vec[j].b + 1);
    if (one < two)
      vec.emplace_back(one), result.emplace_back(one.val), ++i;
    else if (one == two)
      vec.emplace_back(one), result.emplace_back(one.val), ++i, ++j;
    else
      vec.emplace_back(two), result.emplace_back(two.val), ++j;
  }
  return result;
}

vector<double> GenerateFirstKABSqrt2BST(int k) {
  std::set<ABSqrt2> set;
  set.emplace(0, 0);
  vector<double> result;
  result.reserve(k);
  while (k--) {
    auto front = *set.cbegin();
    set.erase(set.cbegin());
    set.emplace(front.a + 1, front.b), set.emplace(front.a, front.b + 1);
    result.emplace_back(front.val);
  }
  return result;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"k"};
  return GenericTestMain(args, "a_b_sqrt2.cc", "a_b_sqrt2.tsv",
                         &GenerateFirstKABSqrt2, DefaultComparator{},
                         param_names);
}
