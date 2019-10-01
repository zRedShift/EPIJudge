#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

int NumCombinationsForFinalScore(int final_score, const vector<int> &individual_play_scores) {
  vector<int> combinations(final_score + 1);
  combinations.front() = 1;
  for (const auto x : individual_play_scores)
    for (int i = 0, j = x; j <= final_score; ++i, ++j)
      combinations[j] += combinations[i];
  return combinations.back();
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"final_score", "individual_play_scores"};
  return GenericTestMain(args, "number_of_score_combinations.cc",
                         "number_of_score_combinations.tsv",
                         &NumCombinationsForFinalScore, DefaultComparator{},
                         param_names);
}
