#include <random>
#include <functional>
#include <iterator>
#include <numeric>
#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/random_sequence_checker.h"
#include "test_framework/timed_executor.h"
using std::default_random_engine;
using std::uniform_int_distribution;
using std::random_device;
using std::bind;
using std::swap;
using std::unordered_map;
using std::vector;
// Returns a random k-sized subset of {0, 1, ..., n - 1}.
vector<int> RandomSubset(int n, int k) {
  vector<int> result;
  unordered_map<int, int> perm;
  default_random_engine re((random_device()) ());
  if (!k) {}
  else if (k == 1)
    result.emplace_back(uniform_int_distribution{0, n - 1}(re));
  else if (n > k * 4) {
    result.resize(k);
    perm.reserve(k * 2);
    for (int i = 0; i < k; ++i) {
      int roll = uniform_int_distribution{i, n - 1}(re);
      auto &val = perm.emplace(roll, roll).first->second;
      result[i] = val;
      val = perm.count(i) ? perm[i] : i;
    }
  } else {
    result.resize(n);
    iota(begin(result), end(result), 0);
    for (int i = 0; i < k; ++i)
      swap(result[i], result[uniform_int_distribution{i, n - 1}(re)]);
    result.resize(k);
  }
  return result;
}

bool RandomSubsetRunner(TimedExecutor &executor, int n, int k) {
  vector<vector<int>> results;

  executor.Run([&] {
    std::generate_n(back_inserter(results), 100000,
                    std::bind(RandomSubset, n, k));
  });

  int total_possible_outcomes = BinomialCoefficient(n, k);
  vector<int> A(n);
  iota(begin(A), end(A), 0);
  vector<vector<int>> combinations;
  for (int i = 0; i < BinomialCoefficient(n, k); ++i) {
    combinations.emplace_back(ComputeCombinationIdx(A, n, k, i));
  }
  vector<int> sequence;
  for (vector<int> result : results) {
    sort(begin(result), end(result));
    sequence.emplace_back(
        distance(begin(combinations),
                 find(begin(combinations), end(combinations), result)));
  }
  return CheckSequenceIsUniformlyRandom(sequence, total_possible_outcomes,
                                        0.01);
}

void RandomSubsetWrapper(TimedExecutor &executor, int n, int k) {
  RunFuncWithRetries(bind(RandomSubsetRunner, std::ref(executor), n, k));
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "n", "k"};
  return GenericTestMain(args, "random_subset.cc", "random_subset.tsv",
                         &RandomSubsetWrapper, DefaultComparator{},
                         param_names);
}
