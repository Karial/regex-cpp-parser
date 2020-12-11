#include "finite_automata.hpp"
#include <fstream>
#include <gtest/gtest.h>

TEST(SMALL_TESTS, TEST1) {
  std::stringstream in("a*b+");
  auto result = CreateASTFromStream(&in);
  auto nfa = CreateNFAFromAST(result.get());
  ASSERT_TRUE(nfa.Check("ab"));
  ASSERT_FALSE(nfa.Check("abc"));
  ASSERT_FALSE(nfa.Check("a"));
  ASSERT_TRUE(nfa.Check("b"));
  ASSERT_TRUE(nfa.Check("aaaaabbbbbbbbbbbbb"));
  auto dfa = CreateDFAFromNFA(nfa);
  ASSERT_TRUE(dfa.Check("ab"));
  ASSERT_FALSE(dfa.Check("abc"));
  ASSERT_FALSE(dfa.Check("a"));
  ASSERT_TRUE(dfa.Check("b"));
  ASSERT_TRUE(dfa.Check("aaaaabbbbbbbbbbbbb"));
}

TEST(SMALL_TESTS, TEST2) {
  std::stringstream in("(a*b+)|c");
  auto result = CreateASTFromStream(&in);
  auto nfa = CreateNFAFromAST(result.get());
  ASSERT_TRUE(nfa.Check("ab"));
  ASSERT_TRUE(nfa.Check("c"));
  ASSERT_FALSE(nfa.Check("a"));
  ASSERT_TRUE(nfa.Check("b"));
  ASSERT_TRUE(nfa.Check("aaaaabbbbbbbbbbbbb"));
  auto dfa = CreateDFAFromNFA(nfa);
  ASSERT_TRUE(dfa.Check("ab"));
  ASSERT_TRUE(dfa.Check("c"));
  ASSERT_FALSE(dfa.Check("a"));
  ASSERT_TRUE(dfa.Check("b"));
  ASSERT_TRUE(dfa.Check("aaaaabbbbbbbbbbbbb"));
}

TEST(SMALL_TESTS, TEST3) {
  std::stringstream in("(a+|b*)");
  auto result = CreateASTFromStream(&in);
  auto nfa = CreateNFAFromAST(result.get());
  ASSERT_TRUE(nfa.Check(""));
  ASSERT_FALSE(nfa.Check("c"));
  ASSERT_TRUE(nfa.Check("a"));
  ASSERT_TRUE(nfa.Check("b"));
  ASSERT_FALSE(nfa.Check("aaaaabbbbbbbbbbbbb"));
  auto dfa = CreateDFAFromNFA(nfa);
  ASSERT_TRUE(dfa.Check(""));
  ASSERT_FALSE(dfa.Check("c"));
  ASSERT_TRUE(dfa.Check("a"));
  ASSERT_TRUE(dfa.Check("b"));
  ASSERT_FALSE(dfa.Check("aaaaabbbbbbbbbbbbb"));
}

TEST(TIMES, TEST1) {
  std::stringstream in("(a+|b*)");
  auto result = CreateASTFromStream(&in);
  auto nfa = CreateNFAFromAST(result.get());
  auto dfa = CreateDFAFromNFA(nfa);
  Benchmark(
      [&nfa]() {
        std::string test;
        for (size_t i = 0; i < 100000; ++i) {
          test += "a";
        }
        for (size_t i = 0; i < 100000; ++i) {
          test += "b";
        }
        nfa.Check(test);
      },
      "NFA with regex (a+|b*)");
  Benchmark(
      [&dfa]() {
        std::string test;
        for (size_t i = 0; i < 100000; ++i) {
          test += "a";
        }
        for (size_t i = 0; i < 100000; ++i) {
          test += "b";
        }
        dfa.Check(test);
      },
      "DFA with regex (a+|b*)");
}

TEST(TIMES, TEST2) {
  std::stringstream in("((a*|b*|c*)d+e+)|(x*y+z*)");
  auto result = CreateASTFromStream(&in);
  auto nfa = CreateNFAFromAST(result.get());
  auto dfa = CreateDFAFromNFA(nfa);
  Benchmark(
      [&nfa]() {
        std::string test;
        for (size_t i = 0; i < 100000; ++i) {
          test += "a";
        }
        for (size_t i = 0; i < 100000; ++i) {
          test += "b";
        }
        nfa.Check(test);
      },
      "NFA with regex (a+|b*)");
  Benchmark(
      [&dfa]() {
        std::string test;
        for (size_t i = 0; i < 100000; ++i) {
          test += "a";
        }
        for (size_t i = 0; i < 100000; ++i) {
          test += "b";
        }
        dfa.Check(test);
      },
      "DFA with regex (a+|b*)");
}

TEST(TIMES, TEST3) {
  std::stringstream in("((((((((a*)+)+)*)+)*)*)+)+");
  auto result = CreateASTFromStream(&in);
  auto nfa = CreateNFAFromAST(result.get());
  auto dfa = CreateDFAFromNFA(nfa);
  Benchmark(
      [&nfa]() {
        std::string test;
        for (size_t i = 0; i < 200000; ++i) {
          test += "a";
        }
        nfa.Check(test);
      },
      "NFA with regex (a+|b*)");
  Benchmark(
      [&dfa]() {
        std::string test;
        for (size_t i = 0; i < 200000; ++i) {
          test += "a";
        }
        dfa.Check(test);
      },
      "DFA with regex (a+|b*)");
}
