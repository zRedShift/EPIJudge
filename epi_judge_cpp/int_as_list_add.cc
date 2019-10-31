#include "list_node.h"
#include "test_framework/generic_test.h"

shared_ptr<ListNode<int>> AddTwoNumbers(shared_ptr<ListNode<int>> L1,
                                        shared_ptr<ListNode<int>> L2) {
  if (!L1)
    return L2;
  else if (!L2)
    return L1;
  auto result = L1;
  int sum = L1->data + L2->data;
  bool carry = sum != (L1->data = sum % 10);
  while (L1->next && L2->next)
    L1->data = (sum = (L1 = L1->next)->data + (L2 = L2->next)->data + carry) % 10, carry = sum >= 10;
  if (!L1->next)
    L1->next = L2->next;
  while (carry && L1->next)
    carry = !(L1->data = ((L1 = L1->next)->data + 1) % 10);
  if (carry)
    L1->next = std::make_shared<ListNode<int>>(1);
  return result;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"L1", "L2"};
  return GenericTestMain(args, "int_as_list_add.cc", "int_as_list_add.tsv",
                         &AddTwoNumbers, DefaultComparator{}, param_names);
}
