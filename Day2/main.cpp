#include "aoc/helpers.h"
#include <vector>

namespace {
  using Result = std::pair<int, std::string>;
  using MappedFileSource = aoc::MappedFileSource<char>;

  constexpr std::string_view SampleInput(R"(ULL
RRDDD
LURDL
UUUUD)");
  constexpr int SR_Part1 = 1985;
  constexpr std::string_view SR_Part2{"5DB3"};

  const std::vector<std::vector<int8_t>> KeyPad{
    { 7, 8, 9 },
    { 4, 5, 6 },
    { 1, 2, 3 },
  };

  const std::vector<std::vector<int8_t>> KeyPad2{
    { 0, 0, 'D', 0, 0 },
    { 0, 'A', 'B', 'C', 0 },
    { '5', '6', '7', '8', '9' },
    { 0, '2', '3', '4', 0 },
    { 0, 0, '1', 0, 0 },
  };


  enum class Direction {
    Up = 0,
    Down,
    Left,
    Right,
  };

  const auto ParseDirection = [](char c) {
    switch (c) {
      case 'U':
        return Direction::Up;
      case 'D':
        return Direction::Down;
      case 'L':
        return Direction::Left;
      case 'R':
        return Direction::Right;
      default:
        throw std::runtime_error("Bad input: " + std::string(1, c));
    }
  };

  aoc::Point Steps[] = {
    { 0, 1 },
    { 0, -1 },
    { -1, 0 },
    { 1, 0 },
  };

  const auto LoadInput = [](auto f) {
    Result r{0, ""};
    std::string_view line;
    aoc::Point pos = { 1, 1 };
    aoc::Point pos2 = { 0, 2 };
    while (aoc::getline(f, line)) {
      for (const auto c : line) {
        assert(c == 'U' || c == 'D' || c == 'L' || c == 'R');
        const auto dir = ParseDirection(c);
        pos += Steps[static_cast<int>(dir)];
        const auto last = pos2;
        pos2 += Steps[static_cast<int>(dir)];

        pos.second = std::max(std::min(pos.second, (int)(KeyPad.size()) - 1), 0);
        pos.first = std::max(std::min(pos.first, (int)(KeyPad.at(pos.second).size()) - 1), 0);

        pos2.second = std::max(std::min(pos2.second, (int)(KeyPad2.size()) - 1), 0);
        pos2.first = std::max(std::min(pos2.first, (int)(KeyPad2.at(pos2.second).size()) - 1), 0);

        if (KeyPad2.at(pos2.second).at(pos2.first) == 0) {
          pos2 = last;
        }
        DEBUG_LOG(pos2.first, pos2.second, KeyPad2.at(pos2.second).at(pos2.first));

        assert(pos.second >= 0 && pos.second < (int)KeyPad.size());
        assert(pos.first >= 0 && pos.first < (int)KeyPad.at(pos.second).size());
        assert(pos2.second >= 0 && pos2.second < (int)KeyPad2.size());
        assert(pos2.first >= 0 && pos2.first < (int)KeyPad2.at(pos2.second).size());
      }
      DEBUG_LOG((int)KeyPad.at(pos.second).at(pos.first));
      DEBUG_LOG(KeyPad2.at(pos2.second).at(pos2.first));

      r.first *= 10;
      r.first += KeyPad.at(pos.second).at(pos.first);
      r.second.append(1, KeyPad2.at(pos2.second).at(pos2.first));
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
  std::string part2;

  std::tie(part1, part2) = r;

  aoc::print_results(part1, part2);

  if (inTest) {
    aoc::assert_result(part1, SR_Part1);
    aoc::assert_result(part2, SR_Part2);
  }

  return 0;
}
