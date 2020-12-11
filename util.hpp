#ifndef REGEX_PARSER_UTIL_HPP
#define REGEX_PARSER_UTIL_HPP

#include <chrono>
#include <iostream>

template <typename Container> struct SetHash {
  size_t operator()(const Container &container) const {
    size_t result = 0;
    for (const auto &i : container) {
      result ^= std::hash<typename Container::value_type>()(i);
    }
    return result;
  }
};

template <typename Function>
void Benchmark(const Function &function, const std::string &functionName) {
  using namespace std::chrono;
  auto start = high_resolution_clock::now();
  function();
  auto stop = high_resolution_clock::now();
  std::cout << "Execution time of " + functionName + " "
            << duration_cast<microseconds>(stop - start).count() << " mcs\n";
}

#endif // REGEX_PARSER_UTIL_HPP
