#include <memory>
#include <cassert>
#include <iostream>

using std::boolalpha;
using std::cout;
using std::endl;
using std::unique_ptr;
using std::make_unique;

class BinaryTreeNode {
 public:
  [[nodiscard]] bool IsLocked() const { return locked; }
  bool Lock() {
    if (locked || num_locked)
      return false;
    locked = true;
    for (auto iter = parent; iter && (locked = !iter->locked); iter = iter->parent) continue;
    if (locked)
      for (auto iter = parent; iter; iter = iter->parent)
        ++iter->num_locked;
    return locked;
  }
  void Unlock() {
    if (locked) {
      locked = false;
      for (auto iter = parent; iter; iter = iter->parent)
        --iter->num_locked;
    }
  }
  unique_ptr<BinaryTreeNode> left, right;
  BinaryTreeNode *parent = nullptr;
 private:
  bool locked = false;
  int num_locked = 0;
};

int main(int argc, char *argv[]) {
  auto root = make_unique<BinaryTreeNode>(BinaryTreeNode());
  root->left = make_unique<BinaryTreeNode>(BinaryTreeNode());
  root->left->parent = root.get();
  root->right = make_unique<BinaryTreeNode>(BinaryTreeNode());
  root->right->parent = root.get();
  root->left->left = make_unique<BinaryTreeNode>(BinaryTreeNode());
  root->left->left->parent = root->left.get();
  root->left->right = make_unique<BinaryTreeNode>(BinaryTreeNode());
  root->left->right->parent = root->left.get();

  assert(!root->IsLocked());
  cout << boolalpha << root->IsLocked() << endl;

  assert(root->Lock());
  assert(root->IsLocked());
  cout << boolalpha << root->IsLocked() << endl;
  assert(!root->left->Lock());
  assert(!root->left->IsLocked());
  assert(!root->right->Lock());
  assert(!root->right->IsLocked());
  assert(!root->left->left->Lock());
  assert(!root->left->left->IsLocked());
  assert(!root->left->right->Lock());
  assert(!root->left->right->IsLocked());

  root->Unlock();
  assert(root->left->Lock());
  assert(!root->Lock());
  assert(!root->left->left->Lock());
  assert(!root->IsLocked());

  cout << boolalpha << root->IsLocked() << endl;
  assert(root->right->Lock());
  assert(root->right->IsLocked());
  cout << boolalpha << root->right->IsLocked() << endl;
  return 0;
}