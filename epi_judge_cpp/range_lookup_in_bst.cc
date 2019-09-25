#include <memory>
#include <vector>
#include "bst_node.h"
#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
using std::unique_ptr;
using std::vector;

struct Interval {
  int left, right;
};

vector<int> RangeLookupInBST(const unique_ptr<BstNode<int>> &tree,
                             const Interval &interval) {
  std::stack<BstNode<int> *> inorder;
  auto iter = tree.get();
  while (iter)
    if (iter->data >= interval.left) {
      if (iter->data <= interval.right)
        inorder.emplace(iter);
      iter = iter->left.get();
    } else
      iter = iter->right.get();
  vector<int> result;
  while (!inorder.empty()) {
    iter = inorder.top(), inorder.pop(), result.emplace_back(iter->data);
    if (iter = iter->right.get(); iter)
      do {
        if (iter->data <= interval.right)
          inorder.emplace(iter);
        iter = iter->left.get();
      } while (iter);
  }
  return result;
}

template<>
struct SerializationTraits<Interval> : UserSerTraits<Interval, int, int> {
  static std::vector<std::string> GetMetricNames(const std::string &arg_name) {
    return {FmtStr("length({})", arg_name)};
  }

  static std::vector<int> GetMetrics(const Interval &x) {
    return {x.right - x.left};
  }
};

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"tree", "interval"};
  return GenericTestMain(args, "range_lookup_in_bst.cc",
                         "range_lookup_in_bst.tsv", &RangeLookupInBST,
                         DefaultComparator{}, param_names);
}
