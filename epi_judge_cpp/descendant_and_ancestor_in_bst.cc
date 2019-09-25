#include <memory>
#include "bst_node.h"
#include "test_framework/binary_tree_utils.h"
#include "test_framework/generic_test.h"
#include "test_framework/timed_executor.h"
using std::unique_ptr;

bool FindDescendant(const BstNode<int> *anc, const BstNode<int> *desc) {
  do
    anc = desc->data < anc->data ? anc->left.get() : anc->right.get();
  while (anc && anc != desc);
  return anc;
}

bool PairIncludesAncestorAndDescendantOfM(
    const unique_ptr<BstNode<int>> &possible_anc_or_desc_0,
    const unique_ptr<BstNode<int>> &possible_anc_or_desc_1,
    const unique_ptr<BstNode<int>> &middle) {
  BstNode<int> *mid = middle.get(), *anc0 = possible_anc_or_desc_0.get(), *anc1 = possible_anc_or_desc_1.get();
  do {
    anc0 = mid->data < anc0->data ? anc0->left.get() : anc0->right.get();
    anc1 = mid->data < anc1->data ? anc1->left.get() : anc1->right.get();
  } while (anc0 && anc1 && anc0 != mid && anc1 != mid);
  if (!anc0)
    return anc1 && (anc1 == mid || FindDescendant(anc1, mid)) && FindDescendant(mid, possible_anc_or_desc_0.get());
  if (!anc1)
    return (anc0 == mid || FindDescendant(anc0, mid)) && FindDescendant(mid, possible_anc_or_desc_1.get());
  return FindDescendant(mid, anc0 == mid ? possible_anc_or_desc_1.get() : possible_anc_or_desc_0.get());

}
bool PairIncludesAncestorAndDescendantOfMWrapper(
    TimedExecutor &executor, const unique_ptr<BstNode<int>> &tree,
    int possible_anc_or_desc_0, int possible_anc_or_desc_1, int middle) {
  auto &candidate0 = MustFindNode(tree, possible_anc_or_desc_0);
  auto &candidate1 = MustFindNode(tree, possible_anc_or_desc_1);
  auto &middle_node = MustFindNode(tree, middle);
  return executor.Run([&] {
    return PairIncludesAncestorAndDescendantOfM(candidate0, candidate1,
                                                middle_node);
  });
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "tree",
                                       "possible_anc_or_desc_0",
                                       "possible_anc_or_desc_1", "middle"};
  return GenericTestMain(args, "descendant_and_ancestor_in_bst.cc",
                         "descendant_and_ancestor_in_bst.tsv",
                         &PairIncludesAncestorAndDescendantOfMWrapper,
                         DefaultComparator{}, param_names);
}
