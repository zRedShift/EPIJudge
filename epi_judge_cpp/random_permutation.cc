#include <functional>
#include <vector>
#include <random>
#include "test_framework/generic_test.h"
#include "test_framework/random_sequence_checker.h"
#include "test_framework/timed_executor.h"
using std::default_random_engine;
using std::uniform_int_distribution;
using std::random_device;
using std::bind;
using std::swap;
using std::vector;
vector<int> ComputeRandomPermutation(int n) {
  default_random_engine re((random_device()) ());
  vector<int> perm(n);
  iota(begin(perm), end(perm), 0);
  int max = n - 1;
  for (int i = 0; i < max; i++)
    swap(perm[i], perm[uniform_int_distribution{i, max}(re)]);
  return perm;
}
int Factorial(int n) { return n <= 1 ? 1 : n * Factorial(n - 1); }

int PermutationIndex(vector<int> perm) {
  int idx = 0;
  int n = perm.size();
  for (int i = 0; i < perm.size(); ++i) {
    int a = perm[i];
    idx += a * Factorial(n - 1);
    for (int j = i + 1; j < perm.size(); ++j) {
      if (perm[j] > a) {
        --perm[j];
      }
    }
    --n;
  }
  return idx;
}

bool ComputeRandomPermutationRunner(TimedExecutor &executor, int n) {
  vector<vector<int>> results;

  executor.Run([&] {
    for (int i = 0; i < 1000000; ++i) {
      results.emplace_back(ComputeRandomPermutation(n));
    }
  });

  vector<int> sequence;
  for (const vector<int> &result : results) {
    sequence.emplace_back(PermutationIndex(result));
  }
  return CheckSequenceIsUniformlyRandom(sequence, Factorial(n), 0.01);
}

void ComputeRandomPermutationWrapper(TimedExecutor &executor, int n) {
  RunFuncWithRetries(
      std::bind(ComputeRandomPermutationRunner, std::ref(executor), n));
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "n"};
  return GenericTestMain(
      args, "random_permutation.cc", "random_permutation.tsv",
      &ComputeRandomPermutationWrapper, DefaultComparator{}, param_names);
}
