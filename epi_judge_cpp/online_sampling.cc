#include <algorithm>
#include <functional>
#include <iterator>
#include <vector>
#include <random>
#include "test_framework/generic_test.h"
#include "test_framework/random_sequence_checker.h"
#include "test_framework/timed_executor.h"
using std::bind;
using std::vector;
using std::default_random_engine;
using std::uniform_int_distribution;
using std::uniform_real_distribution;
using std::swap;
using std::random_device;
using std::random_shuffle;

// Assumption: there are at least k elements in the stream.
vector<int> OnlineRandomSample(vector<int>::const_iterator stream_begin,
                               const vector<int>::const_iterator stream_end,
                               const int k) {
  default_random_engine re((random_device()) ());
  vector<int> result(stream_begin, stream_end);
  shuffle(result.begin(), result.end(), re);
  return {result.begin(), result.begin() + k};
}
bool OnlineRandomSamplingRunner(TimedExecutor &executor, vector<int> stream,
                                int k) {
  vector<vector<int>> results;

  executor.Run([&] {
    std::generate_n(
        back_inserter(results), 100000,
        std::bind(OnlineRandomSample, cbegin(stream), cend(stream), k));
  });

  int total_possible_outcomes = BinomialCoefficient(stream.size(), k);
  sort(begin(stream), end(stream));
  vector<vector<int>> combinations;
  for (int i = 0; i < BinomialCoefficient(stream.size(), k); ++i) {
    combinations.emplace_back(
        ComputeCombinationIdx(stream, stream.size(), k, i));
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

void OnlineRandomSampleWrapper(TimedExecutor &executor,
                               const vector<int> &stream, int k) {
  RunFuncWithRetries(bind(OnlineRandomSamplingRunner, std::ref(executor),
                          std::cref(stream), k));
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "stream", "k"};
  return GenericTestMain(args, "online_sampling.cc", "online_sampling.tsv",
                         &OnlineRandomSampleWrapper, DefaultComparator{},
                         param_names);
}
