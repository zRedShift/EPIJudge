#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
using std::vector;

struct DuplicateAndMissing {
  int duplicate, missing;
};

DuplicateAndMissing FindDuplicateMissing(const vector<int> &A) {
//  int diff = 0, i = 0;
//  for (const auto &num: A) {
//    diff += i - num;
//    i++;
//  }
//  int stride = std::abs(diff);
//  auto different_sign = [stride](const int x) { return (1 - x / stride % 2 * 2) * x; };
//  i = 0;
//  int sum = 0;
//  for (const auto &num: A) {
//    sum += different_sign(i) - different_sign(num);
//    i++;
//  }
//  sum = std::abs(sum);
//  return {(sum - diff) / 2, (sum + diff) / 2};
  unsigned int missing_xor_duplicate = 0;
  for (unsigned int i = 0; i < (unsigned int) size(A); ++i)
    missing_xor_duplicate ^= (unsigned int) A[i] ^ i;
  unsigned int lowest_set_bit = missing_xor_duplicate & ~(missing_xor_duplicate - 1);
  unsigned int set_bit_num = 0;
  for (unsigned int i = 0; i < (unsigned int) size(A); ++i) {
    if ((unsigned int) A[i] & lowest_set_bit) {
      set_bit_num ^= (unsigned int) A[i];
    }
    if (i & lowest_set_bit) {
      set_bit_num ^= i;
    }
  }
  int first = int(set_bit_num), second = int(set_bit_num ^ missing_xor_duplicate);
  if (find(begin(A), end(A), first) != end(A))
    return {first, second};
  return {second, first};
}
template<>
struct SerializationTraits<DuplicateAndMissing>
    : UserSerTraits<DuplicateAndMissing, int, int> {
};

bool operator==(const DuplicateAndMissing &lhs,
                const DuplicateAndMissing &rhs) {
  return std::tie(lhs.duplicate, lhs.missing) ==
      std::tie(rhs.duplicate, rhs.missing);
}

std::ostream &operator<<(std::ostream &out, const DuplicateAndMissing &x) {
  return out << "duplicate: " << x.duplicate << ", missing: " << x.missing;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(
      args, "search_for_missing_element.cc", "find_missing_and_duplicate.tsv",
      &FindDuplicateMissing, DefaultComparator{}, param_names);
}
