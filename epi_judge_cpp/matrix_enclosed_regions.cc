#include <stdexcept>
#include <string>
#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/timed_executor.h"
using std::string;
using std::vector;

void FillSurroundedRegions(vector<vector<char>> *board_ptr) {
  auto &board = *board_ptr;
  if (board.size() < 3 || board.front().size() < 3)
    return;
  std::deque<std::pair<int, int>> coordinates;
  for (int y = 0; y < board.front().size(); ++y) {
    if (board.front()[y] == 'W')
      board.front()[y] = 'T', coordinates.emplace_back(1, y);
    if (board.back()[y] == 'W')
      board.back()[y] = 'T', coordinates.emplace_back(board.size() - 2, y);
  }
  for (int x = 1; x < board.size() - 1; ++x) {
    if (board[x].front() == 'W')
      board[x].front() = 'T', coordinates.emplace_back(x, 1);
    if (board[x].back() == 'W')
      board[x].back() = 'T', coordinates.emplace_back(x, board.front().size() - 2);
  }
  while (!coordinates.empty()) {
    const auto[x, y] = coordinates.front();
    coordinates.pop_front();
    if (board[x][y] == 'W') {
      board[x][y] = 'T', coordinates.insert(coordinates.end(), {{x - 1, y}, {x + 1, y}, {x, y - 1}, {x, y + 1}});
    }
  }
  for (auto &col : board)
    for (auto &c : col)
      c = c == 'T' ? 'W' : 'B';
}
vector<vector<string>> FillSurroundedRegionsWrapper(
    TimedExecutor &executor, vector<vector<string>> board) {
  vector<vector<char>> char_vector;
  char_vector.resize(board.size());
  for (int i = 0; i < board.size(); i++) {
    for (const string &s : board[i]) {
      if (s.size() != 1) {
        throw std::runtime_error("String size is not 1");
      }
      char_vector[i].push_back(s[0]);
    }
  }

  executor.Run([&] { FillSurroundedRegions(&char_vector); });

  board.clear();
  board.resize(char_vector.size(), {});
  for (int i = 0; i < board.size(); i++) {
    for (char c : char_vector[i]) {
      board[i].emplace_back(1, c);
    }
  }

  return board;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "board"};
  return GenericTestMain(
      args, "matrix_enclosed_regions.cc", "matrix_enclosed_regions.tsv",
      &FillSurroundedRegionsWrapper, DefaultComparator{}, param_names);
}
