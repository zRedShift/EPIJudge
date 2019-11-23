#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
using std::vector;

struct Jug {
  int low, high;
};
bool CheckFeasible(const vector<Jug> &jugs, int L, int H) {
  int D = H - L + 1;
  vector<std::pair<vector<int>, vector<bool>>> dp(L, {vector<int>{}, vector<bool>(D, false)});
  dp[0].second[0] = true, dp[0].first.emplace_back(0);
  for (auto[low, diff] : jugs) {
    diff -= low;
    for (int new_low = low; new_low < L; ++new_low)
      for (const auto d : dp[new_low - low].first)
        if (int new_diff = d + diff; new_diff < D && !dp[new_low].second[new_diff])
          dp[new_low].second[new_diff] = true, dp[new_low].first.emplace_back(new_diff);
    for (int i = std::max(0, L - low), new_low = i + low + diff; i < L; ++i, ++new_low)
      for (const auto d : dp[i].first)
        if (new_low + d <= H)
          return true;
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
