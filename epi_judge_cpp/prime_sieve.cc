#include <vector>
#include "test_framework/generic_test.h"
using std::deque;
using std::vector;
// Given n, return all primes up to and including n.
vector<int> GeneratePrimes(int n) {
  if (n < 2)
    return {};
  ++n;
  vector<int> primes{2};
  int sqrt_n = ceil(sqrt(n));
  deque<bool> not_prime(n/2 + 1, false);
  for (int i = 3; i < sqrt_n; i += 2)
    if (!not_prime[i / 2]) {
      primes.emplace_back(i);
      for (int j = i * i; j < n; j += 2 * i)
        not_prime[j / 2] = true;
    }
  for (int i = sqrt_n + 1 - sqrt_n % 2; i < n; i += 2  )
    if (!not_prime[i / 2])
      primes.emplace_back(i);
  return primes;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"n"};
  return GenericTestMain(args, "prime_sieve.cc", "prime_sieve.tsv",
                         &GeneratePrimes, DefaultComparator{}, param_names);
}
