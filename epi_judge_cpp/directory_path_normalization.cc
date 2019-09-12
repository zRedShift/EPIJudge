#include <string>
#include "test_framework/generic_test.h"
using std::string;
string ShortestEquivalentPath(const string &path) {
  string s;
  bool root = false;
  if (path.front() == '/') {
    s += '/';
    root = true;
  }
  for (size_t i = root; i < path.size(); ++i)
    if (path[i] >= '0')
      s += path[i];
    else if (path[i] == '.') {
      if (path[i + 1] != '.') {
        ++i;
        continue;
      } else if (s.empty() || s[size(s) - 2] == '.')
        s += "../";
      else {
        auto n = s.rfind('/', s.size() - 2);
        s.resize(n == string::npos ? 0 : n + 1);
      }
      i += 2;
    } else if (!s.empty() && s.back() != '/')
      s += '/';
  if (s.back() == '/' && s.size() != 1)
    s.pop_back();
  return s;
}

string ShortestEquivalentPathSlower(const string &path) {
  std::vector<string> v;
  std::stringstream ss(path);
  string token;
  const char kDelimiter = '/';
  if (ss.peek() == kDelimiter) {
    v.emplace_back("");
    ss.ignore();
  }
  while (getline(ss, token, kDelimiter))
    if (token.empty() || token == ".")
      continue;
    else if (v.empty() || token != ".." || v.back() == token)
      v.emplace_back(token);
    else
      v.pop_back();
  if (v.back().empty())
    return "/";
  string s = std::move(v[0]);
  for (auto p = std::next(v.begin()); p != v.end(); ++p)
    s += '/', s += *p;
  return s;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"path"};
  return GenericTestMain(args, "directory_path_normalization.cc",
                         "directory_path_normalization.tsv",
                         &ShortestEquivalentPath, DefaultComparator{},
                         param_names);
}
