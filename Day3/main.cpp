#include "aoc/helpers.h"
#include <vector>

namespace {
  using Result = std::pair<int, int>;
  using MappedFileSource = aoc::MappedFileSource<char>;

  constexpr std::string_view SampleInput(R"(5 10 25
15 15 25
25 15 10)");
  constexpr int SR_Part1 = 1;
  constexpr int SR_Part2 = 2;

  const auto isValidTriangle = [](const std::vector<int>& t) {
    assert(t.size() == 3);
    return t.at(0) + t.at(1) > t.at(2) &&
        t.at(1) + t.at(2) > t.at(0) &&
        t.at(0) + t.at(2) > t.at(1);
};

  const auto LoadInput = [](auto f) {
    Result r{0, 0};
    std::string_view line;
    std::vector<int> triangle;
    std::vector<std::vector<int>> vtriangles;
    triangle.reserve(3);
    vtriangles.resize(3);
    vtriangles.at(0).reserve(3);
    vtriangles.at(1).reserve(3);
    vtriangles.at(2).reserve(3);
    while (aoc::getline(f, line)) {
      triangle.clear();
      aoc::parse_as_integers(line, " ", [&triangle](const int64_t v) {
        triangle.push_back(v);
      });

      r.first += isValidTriangle(triangle);
      for (size_t v = 0; v < 3; v++) {
        auto &t = vtriangles.at(v);
        t.push_back(triangle.at(v));

        if (t.size() == 3) {
          r.second += isValidTriangle(t);
          t.clear();
        }
      }
    }
    return r;
  };
}

int main(int argc, char** argv) {
  aoc::AutoTimer t;
  const bool inTest = argc < 2;

  Result r;
  if (inTest) {
    r = LoadInput(SampleInput);
  } else {
    std::unique_ptr<MappedFileSource>m(new MappedFileSource(argc, argv));
    std::string_view f(m->data(), m->size());
    r = LoadInput(f);
  }

  int part1 = 0;
  int part2 = 0;

  std::tie(part1, part2) = r;

  aoc::print_results(part1, part2);

  if (inTest) {
    aoc::assert_result(part1, SR_Part1);
    aoc::assert_result(part2, SR_Part2);
  }

  return 0;
}
