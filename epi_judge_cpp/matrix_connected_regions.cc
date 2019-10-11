#include <deque>
#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/timed_executor.h"
using std::deque;
using std::vector;

void FlipColor(int x, int y, vector<deque<bool>> *image_ptr) {
  auto &image = *image_ptr;
  const int x_max = image.size(), y_max = image.front().size();
  const bool color = image[x][y] = !image[x][y];
  deque<std::pair<int, int>> coordinates{{x - 1, y}, {x + 1, y}, {x, y - 1}, {x, y + 1}};
  do {
    std::tie(x, y) = coordinates.front();
    coordinates.pop_front();
    if (x >= 0 && x < x_max && y >= 0 && y < y_max && image[x][y] != color)
      image[x][y] = color, coordinates.insert(coordinates.end(), {{x - 1, y}, {x + 1, y}, {x, y - 1}, {x, y + 1}});
  } while (!coordinates.empty());
}
vector<vector<int>> FlipColorWrapper(TimedExecutor &executor, int x, int y,
                                     vector<vector<int>> image) {
  vector<deque<bool>> b;
  b.reserve(image.size());
  for (const vector<int> &row : image) {
    deque<bool> tmp;
    tmp.resize(row.size());
    for (int i = 0; i < row.size(); ++i) {
      tmp[i] = static_cast<bool>(row[i]);
    }
    b.push_back(tmp);
  }

  executor.Run([&] { FlipColor(x, y, &b); });

  image.resize(b.size());

  for (int i = 0; i < image.size(); ++i) {
    image[i].resize(b.size());
    for (int j = 0; j < image[i].size(); ++j) {
      image[i][j] = b[i][j];
    }
  }
  return image;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "x", "y", "image"};
  return GenericTestMain(args, "matrix_connected_regions.cc", "painting.tsv",
                         &FlipColorWrapper, DefaultComparator{}, param_names);
}
