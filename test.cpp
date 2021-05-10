#include "finite_automata.hpp"
#include <fstream>
#include <gtest/gtest.h>

TEST(BASIC_FUNCTIONALITY_TESTS, TEST1) {
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

TEST(BASIC_FUNCTIONALITY_TESTS, TEST2) {
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

TEST(BASIC_FUNCTIONALITY_TESTS, TEST3) {
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

TEST(BASIC_FUNCTIONALITY_TESTS, TEST4) {
  std::stringstream in("\\(a*\\)");
  auto ast = CreateASTFromStream(&in);
  auto nfa = CreateNFAFromAST(ast.get());
  ASSERT_TRUE(nfa.Check("()"));
  ASSERT_FALSE(nfa.Check("c"));
  ASSERT_TRUE(nfa.Check("(a)"));
  ASSERT_TRUE(nfa.Check("(aa)"));
  ASSERT_FALSE(nfa.Check("b"));
  ASSERT_FALSE(nfa.Check("aaaaabbbbbbbbbbbbb"));
  auto dfa = CreateDFAFromNFA(nfa);
  ASSERT_TRUE(dfa.Check("()"));
  ASSERT_FALSE(dfa.Check("c"));
  ASSERT_TRUE(dfa.Check("(a)"));
  ASSERT_TRUE(dfa.Check("(aa)"));
  ASSERT_FALSE(dfa.Check("b"));
  ASSERT_FALSE(dfa.Check("aaaaabbbbbbbbbbbbb"));
}

TEST(BASIC_FUNCTIONALITY_TESTS, TEST5) {
  std::stringstream in("\\*+");
  auto ast = CreateASTFromStream(&in);
  auto nfa = CreateNFAFromAST(ast.get());
  ASSERT_TRUE(nfa.Check("***"));
  ASSERT_FALSE(nfa.Check("c"));
  auto dfa = CreateDFAFromNFA(nfa);
  ASSERT_TRUE(dfa.Check("***"));
  ASSERT_FALSE(dfa.Check("c"));
}

TEST(CHARACTER_GROUPS, DIGITS) {
  std::stringstream in("\\d+");
  auto ast = CreateASTFromStream(&in);
  auto nfa = CreateNFAFromAST(ast.get());
  ASSERT_TRUE(nfa.Check("1231"));
  ASSERT_TRUE(nfa.Check("12"));
  ASSERT_TRUE(nfa.Check("7"));
  ASSERT_FALSE(nfa.Check("7fgfdg"));
  ASSERT_FALSE(nfa.Check("a723"));
  ASSERT_FALSE(nfa.Check("-a711"));
  ASSERT_FALSE(nfa.Check("-fgfdg"));
  auto dfa = CreateDFAFromNFA(nfa);
  ASSERT_TRUE(dfa.Check("1231"));
  ASSERT_TRUE(dfa.Check("12"));
  ASSERT_TRUE(dfa.Check("7"));
  ASSERT_FALSE(dfa.Check("7fgfdg"));
  ASSERT_FALSE(dfa.Check("a723"));
  ASSERT_FALSE(dfa.Check("-a711"));
  ASSERT_FALSE(dfa.Check("-fgfdg"));

  for (char c = 1; c < 127; ++c) {
    if (isdigit(c)) {
      ASSERT_TRUE(nfa.Check(std::string({c})));
      ASSERT_TRUE(dfa.Check(std::string({c})));
    } else {
      ASSERT_FALSE(nfa.Check(std::string({c})));
      ASSERT_FALSE(dfa.Check(std::string({c})));
    }
  }
}

TEST(CHARACTER_GROUPS, NON_DIGITS) {
  std::stringstream in("\\D+");
  auto ast = CreateASTFromStream(&in);
  auto nfa = CreateNFAFromAST(ast.get());
  ASSERT_FALSE(nfa.Check("sdkf6sdfbdsf"));
  ASSERT_FALSE(nfa.Check("23dfg"));
  ASSERT_FALSE(nfa.Check("dfg561"));
  ASSERT_TRUE(nfa.Check("ddfgfdg?;lsdffg"));
  ASSERT_TRUE(nfa.Check("dfgfgsfgfddfg"));
  ASSERT_TRUE(nfa.Check("dfg;l,;lmfgb"));
  auto dfa = CreateDFAFromNFA(nfa);
  ASSERT_FALSE(dfa.Check("sdkf6sdfbdsf"));
  ASSERT_FALSE(dfa.Check("23dfg"));
  ASSERT_FALSE(dfa.Check("dfg561"));
  ASSERT_TRUE(dfa.Check("ddfgfdg?;lsdffg"));
  ASSERT_TRUE(dfa.Check("dfgfgsfgfddfg"));
  ASSERT_TRUE(dfa.Check("dfg;l,;lmfgb"));

  for (char c = 1; c < 127; ++c) {
    if (!isdigit(c)) {
      ASSERT_TRUE(nfa.Check(std::string({c})));
      ASSERT_TRUE(dfa.Check(std::string({c})));
    } else {
      ASSERT_FALSE(nfa.Check(std::string({c})));
      ASSERT_FALSE(dfa.Check(std::string({c})));
    }
  }
}

TEST(CHARACTER_GROUPS, WORD_CHARACTERS) {
  std::stringstream in("\\w+");
  auto ast = CreateASTFromStream(&in);
  auto nfa = CreateNFAFromAST(ast.get());
  ASSERT_TRUE(nfa.Check("sdkf6sdfbdsf"));
  ASSERT_TRUE(nfa.Check("23dfg"));
  ASSERT_TRUE(nfa.Check("dfg561"));
  ASSERT_FALSE(nfa.Check("ddfgfdg?;lsdffg"));
  ASSERT_FALSE(nfa.Check("dfgfgsf,gfd!dfg"));
  ASSERT_FALSE(nfa.Check("dfg;l,;lmfgb"));
  auto dfa = CreateDFAFromNFA(nfa);
  ASSERT_TRUE(dfa.Check("sdkf6sdfbdsf"));
  ASSERT_TRUE(dfa.Check("23dfg"));
  ASSERT_TRUE(dfa.Check("dfg561"));
  ASSERT_FALSE(dfa.Check("ddfgfdg?;lsdffg"));
  ASSERT_FALSE(dfa.Check("dfgfgsf,gfd!dfg"));
  ASSERT_FALSE(dfa.Check("dfg;l,;lmfgb"));

  for (char c = 1; c < 127; ++c) {
    if (isalnum(c) || c == '_') {
      ASSERT_TRUE(nfa.Check(std::string({c})));
      ASSERT_TRUE(dfa.Check(std::string({c})));
    } else {
      ASSERT_FALSE(nfa.Check(std::string({c})));
      ASSERT_FALSE(dfa.Check(std::string({c})));
    }
  }
}

TEST(CHARACTER_GROUPS, NON_WORD_CHARACTERS) {
  std::stringstream in("\\W+");
  auto ast = CreateASTFromStream(&in);
  auto nfa = CreateNFAFromAST(ast.get());
  ASSERT_FALSE(nfa.Check("sdkf6sdfbdsf"));
  ASSERT_FALSE(nfa.Check("23dfg"));
  ASSERT_FALSE(nfa.Check("--!dfg561"));
  ASSERT_TRUE(nfa.Check("??!?@--!@?"));
  ASSERT_TRUE(nfa.Check("-?+--!---?"));
  ASSERT_TRUE(nfa.Check("+-\\(\\)"));
  auto dfa = CreateDFAFromNFA(nfa);
  ASSERT_FALSE(dfa.Check("sdkf6sdfbdsf"));
  ASSERT_FALSE(dfa.Check("23dfg"));
  ASSERT_FALSE(dfa.Check("--!dfg561"));
  ASSERT_TRUE(dfa.Check("??!?@--!@?"));
  ASSERT_TRUE(dfa.Check("-?+--!---?"));
  ASSERT_TRUE(dfa.Check("+-\\(\\)"));

  for (char c = 1; c < 127; ++c) {
    if (!(isalnum(c) || c == '_')) {
      ASSERT_TRUE(nfa.Check(std::string({c})));
      ASSERT_TRUE(dfa.Check(std::string({c})));
    } else {
      ASSERT_FALSE(nfa.Check(std::string({c})));
      ASSERT_FALSE(dfa.Check(std::string({c})));
    }
  }
}

TEST(CHARACTER_GROUPS, SPACE_CHARACTERS) {
  std::stringstream in("\\s+");
  auto ast = CreateASTFromStream(&in);
  auto nfa = CreateNFAFromAST(ast.get());
  ASSERT_FALSE(nfa.Check("\n\tdfgfd\f"));
  ASSERT_TRUE(nfa.Check("\n\t\f\r"));
  auto dfa = CreateDFAFromNFA(nfa);
  ASSERT_FALSE(dfa.Check("\n\tdfgfd\f"));
  ASSERT_TRUE(dfa.Check("\n\t\f\r"));
}

TEST(CHARACTER_GROUPS, NON_SPACE_CHARACTERS) {
  std::stringstream in("\\S+");
  auto ast = CreateASTFromStream(&in);
  auto nfa = CreateNFAFromAST(ast.get());
  ASSERT_TRUE(nfa.Check("dfgfd"));
  ASSERT_FALSE(nfa.Check("\ndfgfd"));
  ASSERT_FALSE(nfa.Check("\tdfgfd"));
  ASSERT_FALSE(nfa.Check("\rdfgfd"));
  ASSERT_FALSE(nfa.Check("\fdfgfd"));
  auto dfa = CreateDFAFromNFA(nfa);
  ASSERT_TRUE(dfa.Check("dfgfd"));
  ASSERT_FALSE(dfa.Check("\ndfgfd"));
  ASSERT_FALSE(dfa.Check("\tdfgfd"));
  ASSERT_FALSE(dfa.Check("\rdfgfd"));
  ASSERT_FALSE(dfa.Check("\fdfgfd"));
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
  Benchmark(
      []() {
        std::string test;
        for (size_t i = 0; i < 200000; ++i) {
          test += "a";
        }
      },
      "Simple loop");
}
