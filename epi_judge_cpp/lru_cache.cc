#include <list>
#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include "test_framework/test_failure.h"
class LruCache {
 public:
  explicit LruCache(size_t capacity) : cache(capacity), capacity(capacity) {}
  int Lookup(int isbn) {
    auto found = cache.find(isbn);
    if (found == cache.end())
      return -1;
    lru.erase(found->second.second);
    lru.push_front(isbn);
    found->second.second = lru.begin();
    return found->second.first;
  }
  void Insert(int isbn, int price) {
    if (Lookup(isbn) > -1)
      return;
    if (cache.size() == capacity) {
      cache.erase(lru.back());
      lru.pop_back();
    }
    lru.push_front(isbn);
    cache.emplace(isbn, std::make_pair(price, lru.begin()));
  }
  bool Erase(int isbn) {
    auto found = cache.find(isbn);
    if (found == cache.end())
      return false;
    lru.erase(found->second.second);
    cache.erase(found);
    return true;
  }
 private:
  size_t capacity;
  std::unordered_map<int, std::pair<int, std::list<int>::iterator>> cache;
  std::list<int> lru;
};
struct Op {
  std::string code;
  int arg1;
  int arg2;
};

template<>
struct SerializationTraits<Op> : UserSerTraits<Op, std::string, int, int> {};

void RunTest(const std::vector<Op> &commands) {
  if (commands.empty() || commands[0].code != "LruCache") {
    throw std::runtime_error("Expected LruCache as first command");
  }
  LruCache cache(commands[0].arg1);

  for (int i = 1; i < commands.size(); i++) {
    auto &cmd = commands[i];
    if (cmd.code == "lookup") {
      int result = cache.Lookup(cmd.arg1);
      if (result != cmd.arg2) {
        throw TestFailure("Lookup: expected " + std::to_string(cmd.arg2) +
            ", got " + std::to_string(result));
      }
    } else if (cmd.code == "insert") {
      cache.Insert(cmd.arg1, cmd.arg2);
    } else if (cmd.code == "erase") {
      bool result = cache.Erase(cmd.arg1);
      if (result != cmd.arg2) {
        throw TestFailure("Erase: expected " + std::to_string(cmd.arg2) +
            ", got " + std::to_string(result));
      }
    } else {
      throw std::runtime_error("Unexpected command " + cmd.code);
    }
  }
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"commands"};
  return GenericTestMain(args, "lru_cache.cc", "lru_cache.tsv", &RunTest,
                         DefaultComparator{}, param_names);
}
