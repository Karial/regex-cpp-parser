#ifndef REGEX_PARSER_UTIL_HPP
#define REGEX_PARSER_UTIL_HPP

#include <chrono>
#include <iostream>

class Range {
 public:
  size_t lowerBound = 0, upperBound = SIZE_T_MAX;
};

template<typename Container>
struct SetHash {
  size_t operator()(const Container &container) const {
    size_t result = 0;
    for (const auto &i : container) {
      result ^= std::hash<typename Container::value_type>()(i);
    }
    return result;
  }
};

template<typename Function>
void Benchmark(const Function &function, const std::string &functionName) {
  using namespace std::chrono;
  auto start = high_resolution_clock::now();
  function();
  auto stop = high_resolution_clock::now();
  std::cout << "Execution time of " + functionName + " "
            << duration_cast<milliseconds>(stop - start).count() << " mcs\n";
}

std::vector<char> CharRange(char first, char last) {
  assert(first <= last);
  std::vector<char> range;
  while (true) {
    range.emplace_back(first);
    if (first == last) {
      break;
    }
    first++;
  }
  return range;
}

#endif // REGEX_PARSER_UTIL_HPP
