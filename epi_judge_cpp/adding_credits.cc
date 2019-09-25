#include <string>
#include "test_framework/fmt_print.h"
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
using std::string;

class ClientsCreditsInfo {
 public:
  void Insert(const string &client_id, int c) {
    int credits_modified = c - credits_baseline;
    if (auto[credits_iter, created] = credits.emplace(client_id, credits_modified); !created) {
      credits_ordered.erase(std::make_pair(credits_iter->second, client_id));
      credits_iter->second = credits_modified;
    }
    credits_ordered.emplace(std::make_pair(credits_modified, client_id));
  }
  bool Remove(const string &client_id) {
    auto credits_iter = credits.find(client_id);
    if (credits_iter == credits.end())
      return false;
    credits_ordered.erase(std::make_pair(credits_iter->second, client_id));
    credits.erase(credits_iter);
    return true;
  }
  [[nodiscard]] int Lookup(const string &client_id) const {
    auto credits_iter = credits.find(client_id);
    return credits_iter == credits.end() ? -1 : credits_baseline + credits_iter->second;
  }
  void AddAll(int C) { credits_baseline += C; }
  [[nodiscard]] string Max() const { return credits_ordered.empty() ? "" : credits_ordered.rbegin()->second; }
 private:
  std::unordered_map<string, int> credits;
  std::set<std::pair<int, string>> credits_ordered;
  int credits_baseline = 0;
};
struct Operation {
  std::string op;
  std::string s_arg;
  int i_arg;
};

std::ostream &operator<<(std::ostream &out, const Operation &op) {
  return out << FmtStr("{}({}, {})", op.op, op.s_arg, op.i_arg);
}

template<>
struct SerializationTraits<Operation>
    : UserSerTraits<Operation, std::string, std::string, int> {
};

void ClientsCreditsInfoTester(const std::vector<Operation> &ops) {
  ClientsCreditsInfo cr;
  int op_idx = 0;
  for (auto &op : ops) {
    if (op.op == "ClientsCreditsInfo") {
      continue;
    } else if (op.op == "remove") {
      bool result = cr.Remove(op.s_arg);
      if (result != op.i_arg) {
        throw TestFailure()
            .WithProperty(PropertyName::STATE, cr)
            .WithProperty(PropertyName::COMMAND, op)
            .WithMismatchInfo(op_idx, op.i_arg, result);
      }
    } else if (op.op == "max") {
      auto result = cr.Max();
      if (result != op.s_arg) {
        throw TestFailure()
            .WithProperty(PropertyName::STATE, cr)
            .WithProperty(PropertyName::COMMAND, op)
            .WithMismatchInfo(op_idx, op.i_arg, result);
      }
    } else if (op.op == "insert") {
      cr.Insert(op.s_arg, op.i_arg);
    } else if (op.op == "add_all") {
      cr.AddAll(op.i_arg);
    } else if (op.op == "lookup") {
      auto result = cr.Lookup(op.s_arg);
      if (result != op.i_arg) {
        throw TestFailure()
            .WithProperty(PropertyName::STATE, cr)
            .WithProperty(PropertyName::COMMAND, op)
            .WithMismatchInfo(op_idx, op.i_arg, result);;
      }
    }
    op_idx++;
  }
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"ops"};
  return GenericTestMain(args, "adding_credits.cc", "adding_credits.tsv",
                         &ClientsCreditsInfoTester, DefaultComparator{},
                         param_names);
}
