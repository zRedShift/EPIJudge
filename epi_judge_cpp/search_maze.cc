#include <istream>
#include <string>
#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"
using std::vector;
typedef enum { kWhite, kBlack } Color;
struct Coordinate {
  bool operator==(const Coordinate &that) const {
    return x == that.x && y == that.y;
  }
  int x, y;
};

vector<Coordinate> SearchMaze(vector<vector<Color>> maze, const Coordinate &s,
                              const Coordinate &e) {
  const int size_x = maze.size() - 1, size_y = maze.front().size() - 1;
  vector<Coordinate> coordinates{s};
  typedef enum { kNorth, kWest, kSouth, kEast, kNone } Direction;
  vector<Direction> directions{kNorth};
  do {
    auto[x, y] = coordinates.back();
    auto &direction = directions.back();
    switch (direction) {
      case kNorth:
        if (maze[x][y] == kBlack) {
          coordinates.pop_back(), directions.pop_back();
          continue;
        }
        if (x == e.x && y == e.y)
          return coordinates;
        maze[x][y] = kBlack;
        if (y != 0) {
          direction = kWest, --y;
          break;
        }
      case kWest:
        if (y != size_y) {
          direction = kSouth, ++y;
          break;
        }
      case kSouth:
        if (x != 0) {
          direction = kEast, --x;
          break;
        }
      case kEast:
        if (x != size_x) {
          direction = kNone, ++x;
          break;
        }
      case kNone:coordinates.pop_back(), directions.pop_back();
        continue;
    }
    coordinates.emplace_back(Coordinate{x, y}), directions.emplace_back(kNorth);
  } while (!coordinates.empty());
  return {};
}
template<>
struct SerializationTraits<Color> : SerializationTraits<int> {
  using serialization_type = Color;

  static serialization_type Parse(const std::string &str) {
    return static_cast<serialization_type>(
        SerializationTraits<int>::Parse(str));
  }

  static serialization_type JsonParse(const json_parser::Json &json_object) {
    return static_cast<serialization_type>(
        SerializationTraits<int>::JsonParse(json_object));
  }
};

template<>
struct SerializationTraits<Coordinate> : UserSerTraits<Coordinate, int, int> {
  static std::vector<std::string> GetMetricNames(const std::string &arg_name) {
    return {};
  }

  static std::vector<int> GetMetrics(const Coordinate &x) { return {}; }
};

bool PathElementIsFeasible(const vector<vector<Color>> &maze,
                           const Coordinate &prev, const Coordinate &cur) {
  if (!(0 <= cur.x && cur.x < maze.size() && 0 <= cur.y &&
      cur.y < maze[cur.x].size() && maze[cur.x][cur.y] == kWhite)) {
    return false;
  }
  return cur == Coordinate{prev.x + 1, prev.y} ||
      cur == Coordinate{prev.x - 1, prev.y} ||
      cur == Coordinate{prev.x, prev.y + 1} ||
      cur == Coordinate{prev.x, prev.y - 1};
}

bool SearchMazeWrapper(TimedExecutor &executor,
                       const vector<vector<Color>> &maze, const Coordinate &s,
                       const Coordinate &e) {
  vector<vector<Color>> copy = maze;

  auto path = executor.Run([&] { return SearchMaze(copy, s, e); });

  if (path.empty()) {
    return s == e;
  }

  if (!(path.front() == s) || !(path.back() == e)) {
    throw TestFailure("Path doesn't lay between start and end points");
  }

  for (size_t i = 1; i < path.size(); i++) {
    if (!PathElementIsFeasible(maze, path[i - 1], path[i])) {
      throw TestFailure("Path contains invalid segments");
    }
  }

  return true;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "maze", "s", "e"};
  return GenericTestMain(args, "search_maze.cc", "search_maze.tsv",
                         &SearchMazeWrapper, DefaultComparator{}, param_names);
}
