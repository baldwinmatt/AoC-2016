#include "aoc/helpers.h"
#include <set>

namespace {
  using Result = std::pair<int, int>;
  using MappedFileSource = aoc::MappedFileSource<char>;

  constexpr std::string_view SampleInput(R"(R8, R4, R4, R8)");
  constexpr int SR_Part1 = 8;
  constexpr int SR_Part2 = 4;

  const auto LoadInput = [](auto f) {
    Result r{0, 0};

    std::set<aoc::Point> visited;
    aoc::Point location{ 0 , 0 };
    aoc::CardinalDirection heading = aoc::CardinalDirection::North;
    std::string_view line;
    DEBUG_LOG(location.first, location.second);
    visited.emplace(location);
    while (aoc::getline(f, line, ", ")) {
      assert(line[0] == 'R' || line[0] == 'L');
      auto d = aoc::stoi(line.substr(1));
      switch (line[0]) {
        case 'L':
          heading = aoc::turnLeft(heading);
          break;
        case 'R':
          heading = aoc::turnRight(heading);
          break;
        default:
          throw std::runtime_error("Bad input: " + std::string(line));
      }
      DEBUG_LOG(static_cast<int32_t>(heading), d);
      while (d > 0) {
        location = aoc::moveInDirection(location, heading, 1);
        if (!visited.emplace(location).second && r.second == 0) {
          r.second = std::abs(location.first) + std::abs(location.second);
        }
        DEBUG_LOG(location.first, location.second);
        d--;
      }
    }
    r.first = std::abs(location.first) + std::abs(location.second);
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
