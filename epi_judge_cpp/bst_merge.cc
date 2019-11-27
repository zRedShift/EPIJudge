#include <memory>
#include "bst_prototype_shared_ptr.h"
#include "test_framework/generic_test.h"
using std::shared_ptr;

std::pair<shared_ptr<BstNode<int>>, shared_ptr<BstNode<int>>> BSTToLinkedList(const shared_ptr<BstNode<int>> &tree) {
  if (!tree)
    return {nullptr, nullptr};
  auto left = BSTToLinkedList(tree->left), right = BSTToLinkedList(tree->right);
  if (left.second)
    left.second->right = tree, tree->left = left.second;
  if (right.first)
    right.first->left = tree, tree->right = right.first;
  return {left.first ? left.first : tree, right.second ? right.second : tree};
}

shared_ptr<BstNode<int>> BSTFromLinkedList(shared_ptr<BstNode<int>> &l, int length) {
  if (!length)
    return nullptr;
  if (length == 1) {
    auto old = l;
    l = l->right, old->left = nullptr, old->right = nullptr;
    return old;
  }
  auto prev = BSTFromLinkedList(l, length / 2);
  l->left = prev, prev = l, l = l->right;
  prev->right = BSTFromLinkedList(l, (length - 1) / 2);
  return prev;
}

shared_ptr<BstNode<int>> MergeTwoBSTs(shared_ptr<BstNode<int>> A, shared_ptr<BstNode<int>> B) {
  if (!A)
    return B;
  if (!B)
    return A;
  A = BSTToLinkedList(A).first, B = BSTToLinkedList(B).first;
  int length = 1;
  if (A->data > B->data)
    std::swap(A, B);
  auto start = A, tail = start;
  A = A->right;
  while (A && B) {
    if (A->data < B->data)
      tail->right = A, A->left = tail, A = A->right;
    else
      tail->right = B, B->left = tail, B = B->right;
    tail = tail->right, ++length;
  }
  if (A) {
    tail->right = A, A->left = tail;
    do {
      A = A->right, ++length;
    } while (A);
  } else if (B) {
    tail->right = B, B->left = tail;
    do {
      B = B->right, ++length;
    } while (B);
  }
  return BSTFromLinkedList(start, length);
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A", "B"};
  return GenericTestMain(args, "bst_merge.cc", "bst_merge.tsv", &MergeTwoBSTs,
                         DefaultComparator{}, param_names);
}
