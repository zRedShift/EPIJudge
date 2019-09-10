#include "list_node.h"
#include "test_framework/generic_test.h"

bool IsLinkedListAPalindrome(const shared_ptr<ListNode<int>> &L) {
  auto iter = L, rev_iter = L;
  while (iter && iter->next)
    iter = iter->next->next, rev_iter = rev_iter->next;
  auto split = iter = rev_iter;
  while (iter) {
    auto temp = iter;
    iter = temp->next;
    temp->next = rev_iter;
    rev_iter = temp;
  }
  iter = L;
  while (iter != split) {
    if (iter->data != rev_iter->data)
      return false;
    iter = iter->next, rev_iter = rev_iter->next;
  }
  return true;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"L"};
  return GenericTestMain(args, "is_list_palindromic.cc",
                         "is_list_palindromic.tsv", &IsLinkedListAPalindrome,
                         DefaultComparator{}, param_names);
}
