#include <string>
#include <vector>
#include "test_framework/generic_test.h"
using std::string;
using std::vector;

int LevenshteinDistance(const string &A, const string &B) {
  auto[small, large] = std::minmax(A, B);
  vector<int> distance(small.size());
  std::iota(distance.begin(), distance.end(), 1);
  for (int i = 0; i < large.size(); ++i) {
    int substitute = i, insert = i + 1;
    for (int j = 0; j < small.size(); ++j)
      if (large[i] == small[j])
        insert = substitute, substitute = distance[j], distance[j] = insert;
      else
        insert = distance[j] = 1 + std::min({substitute, substitute = distance[j], insert});
  }
  return distance.back();
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A", "B"};
  return GenericTestMain(args, "levenshtein_distance.cc",
                         "levenshtein_distance.tsv", &LevenshteinDistance,
                         DefaultComparator{}, param_names);
}
