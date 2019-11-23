#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
using std::vector;

struct Jug {
  int low, high;
};
bool CheckFeasible(const vector<Jug> &jugs, int L, int H) {
  int D = H - L + 1;
  vector<vector<bool>> dp(L, vector<bool>(D, false));
  dp[0][0] = true;
  for (auto[l, d] : jugs) {
    d -= l;
    for (int i = 0; i < L; ++i)
      for (int j = 0; j < D; ++j)
        if (dp[i][j]) {
          if (i + l < L && j + d < D)
            dp[i + l][j + d] = true;
          else if (i + l >= L && i + l + j + d <= H)
            return true;
        }
  }
  return false;
}

template<>
struct SerializationTraits<Jug> : UserSerTraits<Jug, int, int> {};

bool operator==(const Jug &lhs, const Jug &rhs) {
  return lhs.low == rhs.low && lhs.high == rhs.high;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"jugs", "L", "H"};
  return GenericTestMain(args, "defective_jugs.cc", "defective_jugs.tsv",
                         &CheckFeasible, DefaultComparator{}, param_names);
}
