//#define SAVE_MEMORY
#include "finite_automata.hpp"
#include "fast_finite_automata.hpp"
#include <fstream>
#include <gtest/gtest.h>
#include <regex>

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
  auto fastDfa = CreateFastFiniteAutomata(dfa);
  ASSERT_TRUE(fastDfa.Check("ab"));
  ASSERT_FALSE(fastDfa.Check("abc"));
  ASSERT_FALSE(fastDfa.Check("a"));
  ASSERT_TRUE(fastDfa.Check("b"));
  ASSERT_TRUE(fastDfa.Check("aaaaabbbbbbbbbbbbb"));
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
  auto fastDfa = CreateFastFiniteAutomata(dfa);
  ASSERT_TRUE(fastDfa.Check("ab"));
  ASSERT_TRUE(fastDfa.Check("c"));
  ASSERT_FALSE(fastDfa.Check("a"));
  ASSERT_TRUE(fastDfa.Check("b"));
  ASSERT_TRUE(fastDfa.Check("aaaaabbbbbbbbbbbbb"));
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
  auto fastDfa = CreateFastFiniteAutomata(dfa);
  ASSERT_TRUE(fastDfa.Check(""));
  ASSERT_FALSE(fastDfa.Check("c"));
  ASSERT_TRUE(fastDfa.Check("a"));
  ASSERT_TRUE(fastDfa.Check("b"));
  ASSERT_FALSE(fastDfa.Check("aaaaabbbbbbbbbbbbb"));
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
  auto fastDfa = CreateFastFiniteAutomata(dfa);
  ASSERT_TRUE(fastDfa.Check("()"));
  ASSERT_FALSE(fastDfa.Check("c"));
  ASSERT_TRUE(fastDfa.Check("(a)"));
  ASSERT_TRUE(fastDfa.Check("(aa)"));
  ASSERT_FALSE(fastDfa.Check("b"));
  ASSERT_FALSE(fastDfa.Check("aaaaabbbbbbbbbbbbb"));
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
  auto fastDfa = CreateFastFiniteAutomata(dfa);
  ASSERT_TRUE(fastDfa.Check("***"));
  ASSERT_FALSE(fastDfa.Check("c"));
}

TEST(BASIC_FUNCTIONALITY_TESTS, TEST6) {
  std::stringstream in("a?");
  auto ast = CreateASTFromStream(&in);
  auto nfa = CreateNFAFromAST(ast.get());
  ASSERT_TRUE(nfa.Check(""));
  ASSERT_TRUE(nfa.Check("a"));
  ASSERT_FALSE(nfa.Check("aa"));
  ASSERT_FALSE(nfa.Check("c"));
  auto dfa = CreateDFAFromNFA(nfa);
  ASSERT_TRUE(dfa.Check(""));
  ASSERT_TRUE(dfa.Check("a"));
  ASSERT_FALSE(dfa.Check("aa"));
  ASSERT_FALSE(dfa.Check("c"));
  auto fastDfa = CreateFastFiniteAutomata(dfa);
  ASSERT_TRUE(fastDfa.Check(""));
  ASSERT_TRUE(fastDfa.Check("a"));
  ASSERT_FALSE(fastDfa.Check("aa"));
  ASSERT_FALSE(fastDfa.Check("c"));
}

TEST(BASIC_FUNCTIONALITY_TESTS, TEST7) {
  std::stringstream in("\\?");
  auto ast = CreateASTFromStream(&in);
  auto nfa = CreateNFAFromAST(ast.get());
  ASSERT_TRUE(nfa.Check("?"));
  ASSERT_FALSE(nfa.Check("c"));
  auto dfa = CreateDFAFromNFA(nfa);
  ASSERT_TRUE(dfa.Check("?"));
  ASSERT_FALSE(dfa.Check("c"));
  auto fastDfa = CreateFastFiniteAutomata(dfa);
  ASSERT_TRUE(fastDfa.Check("?"));
  ASSERT_FALSE(fastDfa.Check("c"));
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
  auto fastDfa = CreateFastFiniteAutomata(dfa);
  ASSERT_TRUE(fastDfa.Check("1231"));
  ASSERT_TRUE(fastDfa.Check("12"));
  ASSERT_TRUE(fastDfa.Check("7"));
  ASSERT_FALSE(fastDfa.Check("7fgfdg"));
  ASSERT_FALSE(fastDfa.Check("a723"));
  ASSERT_FALSE(fastDfa.Check("-a711"));
  ASSERT_FALSE(fastDfa.Check("-fgfdg"));

  for (char c = 1; c < 127; ++c) {
    if (isdigit(c)) {
      ASSERT_TRUE(nfa.Check(std::string({c})));
      ASSERT_TRUE(dfa.Check(std::string({c})));
      ASSERT_TRUE(fastDfa.Check(std::string({c})));
    } else {
      ASSERT_FALSE(nfa.Check(std::string({c})));
      ASSERT_FALSE(dfa.Check(std::string({c})));
      ASSERT_FALSE(fastDfa.Check(std::string({c})));
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
  auto fastDfa = CreateFastFiniteAutomata(dfa);
  ASSERT_FALSE(fastDfa.Check("sdkf6sdfbdsf"));
  ASSERT_FALSE(fastDfa.Check("23dfg"));
  ASSERT_FALSE(fastDfa.Check("dfg561"));
  ASSERT_TRUE(fastDfa.Check("ddfgfdg?;lsdffg"));
  ASSERT_TRUE(fastDfa.Check("dfgfgsfgfddfg"));
  ASSERT_TRUE(fastDfa.Check("dfg;l,;lmfgb"));

  for (char c = 1; c < 127; ++c) {
    if (!isdigit(c)) {
      ASSERT_TRUE(nfa.Check(std::string({c})));
      ASSERT_TRUE(dfa.Check(std::string({c})));
      ASSERT_TRUE(fastDfa.Check(std::string({c})));
    } else {
      ASSERT_FALSE(nfa.Check(std::string({c})));
      ASSERT_FALSE(dfa.Check(std::string({c})));
      ASSERT_FALSE(fastDfa.Check(std::string({c})));
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
  auto fastDfa = CreateFastFiniteAutomata(dfa);
  ASSERT_TRUE(fastDfa.Check("sdkf6sdfbdsf"));
  ASSERT_TRUE(fastDfa.Check("23dfg"));
  ASSERT_TRUE(fastDfa.Check("dfg561"));
  ASSERT_FALSE(fastDfa.Check("ddfgfdg?;lsdffg"));
  ASSERT_FALSE(fastDfa.Check("dfgfgsf,gfd!dfg"));
  ASSERT_FALSE(fastDfa.Check("dfg;l,;lmfgb"));

  for (char c = 1; c < 127; ++c) {
    if (isalnum(c) || c == '_') {
      ASSERT_TRUE(nfa.Check(std::string({c})));
      ASSERT_TRUE(dfa.Check(std::string({c})));
      ASSERT_TRUE(fastDfa.Check(std::string({c})));
    } else {
      ASSERT_FALSE(nfa.Check(std::string({c})));
      ASSERT_FALSE(dfa.Check(std::string({c})));
      ASSERT_FALSE(fastDfa.Check(std::string({c})));
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
  auto fastDfa = CreateFastFiniteAutomata(dfa);
  ASSERT_FALSE(fastDfa.Check("sdkf6sdfbdsf"));
  ASSERT_FALSE(fastDfa.Check("23dfg"));
  ASSERT_FALSE(fastDfa.Check("--!dfg561"));
  ASSERT_TRUE(fastDfa.Check("??!?@--!@?"));
  ASSERT_TRUE(fastDfa.Check("-?+--!---?"));
  ASSERT_TRUE(fastDfa.Check("+-\\(\\)"));

  for (char c = 1; c < 127; ++c) {
    if (!(isalnum(c) || c == '_')) {
      ASSERT_TRUE(nfa.Check(std::string({c})));
      ASSERT_TRUE(dfa.Check(std::string({c})));
      ASSERT_TRUE(fastDfa.Check(std::string({c})));
    } else {
      ASSERT_FALSE(nfa.Check(std::string({c})));
      ASSERT_FALSE(dfa.Check(std::string({c})));
      ASSERT_FALSE(fastDfa.Check(std::string({c})));
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
  auto fastDfa = CreateFastFiniteAutomata(dfa);
  ASSERT_FALSE(fastDfa.Check("\n\tdfgfd\f"));
  ASSERT_TRUE(fastDfa.Check("\n\t\f\r"));
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
  auto fastDfa = CreateFastFiniteAutomata(dfa);
  ASSERT_TRUE(fastDfa.Check("dfgfd"));
  ASSERT_FALSE(fastDfa.Check("\ndfgfd"));
  ASSERT_FALSE(fastDfa.Check("\tdfgfd"));
  ASSERT_FALSE(fastDfa.Check("\rdfgfd"));
  ASSERT_FALSE(fastDfa.Check("\fdfgfd"));
}

TEST(CHARACTER_GROUPS, CUSTOM_GROUP1) {
  std::stringstream in("[abc]+");
  auto ast = CreateASTFromStream(&in);
  auto nfa = CreateNFAFromAST(ast.get());
  ASSERT_TRUE(nfa.Check("abaccb"));
  ASSERT_FALSE(nfa.Check("abcde"));
  auto dfa = CreateDFAFromNFA(nfa);
  ASSERT_TRUE(dfa.Check("abaccb"));
  ASSERT_FALSE(dfa.Check("abcde"));
  auto fastDfa = CreateFastFiniteAutomata(dfa);
  ASSERT_TRUE(fastDfa.Check("abaccb"));
  ASSERT_FALSE(fastDfa.Check("abcde"));
}

TEST(CHARACTER_GROUPS, CUSTOM_GROUP2) {
  std::stringstream in("[\\{\\}\\(\\)\\[\\]]+");
  auto ast = CreateASTFromStream(&in);
  auto nfa = CreateNFAFromAST(ast.get());
  ASSERT_TRUE(nfa.Check("{()(}{})[][](){}][]["));
  ASSERT_FALSE(nfa.Check("(a)"));
  auto dfa = CreateDFAFromNFA(nfa);
  ASSERT_TRUE(dfa.Check("{()(}{})[][](){}][]["));
  ASSERT_FALSE(dfa.Check("(a)"));
  auto fastDfa = CreateFastFiniteAutomata(dfa);
  ASSERT_TRUE(fastDfa.Check("{()(}{})[][](){}][]["));
  ASSERT_FALSE(fastDfa.Check("(a)"));
}

TEST(CHARACTER_GROUPS, CUSTOM_GROUP3) {
  std::stringstream in("[[]]");
  auto ast = CreateASTFromStream(&in);
  auto nfa = CreateNFAFromAST(ast.get());
  ASSERT_TRUE(nfa.Check("[]"));
  auto dfa = CreateDFAFromNFA(nfa);
  ASSERT_TRUE(dfa.Check("[]"));
  auto fastDfa = CreateFastFiniteAutomata(dfa);
  ASSERT_TRUE(fastDfa.Check("[]"));
}

TEST(CHARACTER_GROUPS, CUSTOM_GROUP4) {
  std::stringstream in("[\\d]");
  auto ast = CreateASTFromStream(&in);
  auto nfa = CreateNFAFromAST(ast.get());
  for (char c = '0'; c < '9'; ++c) {
    ASSERT_TRUE(nfa.Check({c}));
  }
  ASSERT_FALSE(nfa.Check("a"));
  auto dfa = CreateDFAFromNFA(nfa);
  for (char c = '0'; c < '9'; ++c) {
    ASSERT_TRUE(dfa.Check({c}));
  }
  ASSERT_FALSE(dfa.Check("a"));
  auto fastDfa = CreateFastFiniteAutomata(dfa);
  for (char c = '0'; c < '9'; ++c) {
    ASSERT_TRUE(fastDfa.Check({c}));
  }
  ASSERT_FALSE(fastDfa.Check("a"));
}

TEST(CHARACTER_GROUPS, RANGE1) {
  std::stringstream in("[a-e]");
  auto ast = CreateASTFromStream(&in);
  auto nfa = CreateNFAFromAST(ast.get());
  for (char c = 'a'; c <= 'e'; ++c) {
    ASSERT_TRUE(nfa.Check({c}));
  }
  ASSERT_FALSE(nfa.Check({'f'}));
  auto dfa = CreateDFAFromNFA(nfa);
  for (char c = 'a'; c <= 'e'; ++c) {
    ASSERT_TRUE(dfa.Check({c}));
  }
  ASSERT_FALSE(dfa.Check({'f'}));
  auto fastDfa = CreateFastFiniteAutomata(dfa);
  for (char c = 'a'; c <= 'e'; ++c) {
    ASSERT_TRUE(fastDfa.Check({c}));
  }
  ASSERT_FALSE(fastDfa.Check({'f'}));
}

TEST(CHARACTER_GROUPS, RANGE2) {
  std::stringstream in("[0-9]");
  auto ast = CreateASTFromStream(&in);
  auto nfa = CreateNFAFromAST(ast.get());
  for (char c = '0'; c <= '9'; ++c) {
    ASSERT_TRUE(nfa.Check({c}));
  }
  ASSERT_FALSE(nfa.Check({'a'}));
  auto dfa = CreateDFAFromNFA(nfa);
  for (char c = '0'; c <= '9'; ++c) {
    ASSERT_TRUE(dfa.Check({c}));
  }
  ASSERT_FALSE(dfa.Check({'a'}));
  auto fastDfa = CreateFastFiniteAutomata(dfa);
  for (char c = '0'; c <= '9'; ++c) {
    ASSERT_TRUE(fastDfa.Check({c}));
  }
  ASSERT_FALSE(fastDfa.Check({'a'}));
}

TEST(CHARACTER_GROUPS, RANGE3) {
  std::stringstream in("[A-\\\\]");
  auto ast = CreateASTFromStream(&in);
  auto nfa = CreateNFAFromAST(ast.get());
  for (char c = 'A'; c <= '\\'; ++c) {
    ASSERT_TRUE(nfa.Check({c}));
  }
  ASSERT_FALSE(nfa.Check({']'}));
  auto dfa = CreateDFAFromNFA(nfa);
  for (char c = 'A'; c <= '\\'; ++c) {
    ASSERT_TRUE(dfa.Check({c}));
  }
  ASSERT_FALSE(dfa.Check({']'}));
  auto fastDfa = CreateFastFiniteAutomata(dfa);
  for (char c = 'A'; c <= '\\'; ++c) {
    ASSERT_TRUE(fastDfa.Check({c}));
  }
  ASSERT_FALSE(fastDfa.Check({']'}));
}

TEST(CHARACTER_GROUPS, NUMBERS) {
  std::stringstream in("-?\\d+(.\\d+)?");
  auto ast = CreateASTFromStream(&in);
  auto nfa = CreateNFAFromAST(ast.get());
  for (double i = -10.0; i <= 10.0; i += 0.1) {
    ASSERT_TRUE(nfa.Check(std::to_string(i)));
  }
  auto dfa = CreateDFAFromNFA(nfa);
  for (double i = -10.0; i <= 10.0; i += 0.1) {
    ASSERT_TRUE(dfa.Check(std::to_string(i)));
  }
  auto fastDfa = CreateFastFiniteAutomata(dfa);
  for (double i = -10.0; i <= 10.0; i += 0.1) {
    ASSERT_TRUE(fastDfa.Check(std::to_string(i)));
  }
}

TEST(NUM_RANGES, TEST1) {
  std::stringstream in("a{2,3}");
  auto ast = CreateASTFromStream(&in);
  auto nfa = CreateNFAFromAST(ast.get());
  ASSERT_FALSE(nfa.Check("a"));
  ASSERT_TRUE(nfa.Check("aa"));
  ASSERT_TRUE(nfa.Check("aaa"));
  ASSERT_FALSE(nfa.Check("aaaa"));
  auto dfa = CreateDFAFromNFA(nfa);
  ASSERT_FALSE(dfa.Check("a"));
  ASSERT_TRUE(dfa.Check("aa"));
  ASSERT_TRUE(dfa.Check("aaa"));
  ASSERT_FALSE(dfa.Check("aaaa"));
  auto fastDfa = CreateFastFiniteAutomata(dfa);
  ASSERT_FALSE(fastDfa.Check("a"));
  ASSERT_TRUE(fastDfa.Check("aa"));
  ASSERT_TRUE(fastDfa.Check("aaa"));
  ASSERT_FALSE(fastDfa.Check("aaaa"));
}

TEST(NUM_RANGES, TEST2) {
  std::stringstream in("[ab]{2,3}");
  auto ast = CreateASTFromStream(&in);
  auto nfa = CreateNFAFromAST(ast.get());
  ASSERT_FALSE(nfa.Check("a"));
  ASSERT_TRUE(nfa.Check("aa"));
  ASSERT_TRUE(nfa.Check("ab"));
  ASSERT_TRUE(nfa.Check("ba"));
  ASSERT_TRUE(nfa.Check("bb"));
  ASSERT_TRUE(nfa.Check("aba"));
  ASSERT_FALSE(nfa.Check("aaaa"));
  auto dfa = CreateDFAFromNFA(nfa);
  ASSERT_FALSE(dfa.Check("a"));
  ASSERT_TRUE(dfa.Check("aa"));
  ASSERT_TRUE(dfa.Check("ab"));
  ASSERT_TRUE(dfa.Check("ba"));
  ASSERT_TRUE(dfa.Check("bb"));
  ASSERT_TRUE(dfa.Check("aba"));
  ASSERT_FALSE(dfa.Check("aaaa"));
  auto fastDfa = CreateFastFiniteAutomata(dfa);
  ASSERT_FALSE(fastDfa.Check("a"));
  ASSERT_TRUE(fastDfa.Check("aa"));
  ASSERT_TRUE(fastDfa.Check("ab"));
  ASSERT_TRUE(fastDfa.Check("ba"));
  ASSERT_TRUE(fastDfa.Check("bb"));
  ASSERT_TRUE(fastDfa.Check("aba"));
  ASSERT_FALSE(fastDfa.Check("aaaa"));
}

TEST(NUM_RANGES, TEST3) {
  std::stringstream in("a{,3}");
  auto ast = CreateASTFromStream(&in);
  auto nfa = CreateNFAFromAST(ast.get());
  for (int i = 1; i <= 3; ++i) {
    std::string curTest;
    for (int j = 0; j < i; ++j) {
      curTest += 'a';
    }
    ASSERT_TRUE(nfa.Check(curTest));
  }
  ASSERT_FALSE(nfa.Check("aaaa"));
  auto dfa = CreateDFAFromNFA(nfa);
  for (int i = 1; i <= 3; ++i) {
    std::string curTest;
    for (int j = 0; j < i; ++j) {
      curTest += 'a';
    }
    ASSERT_TRUE(dfa.Check(curTest));
  }
  ASSERT_FALSE(dfa.Check("aaaa"));
  auto fastDfa = CreateFastFiniteAutomata(dfa);
  for (int i = 1; i <= 3; ++i) {
    std::string curTest;
    for (int j = 0; j < i; ++j) {
      curTest += 'a';
    }
    ASSERT_TRUE(fastDfa.Check(curTest));
  }
  ASSERT_FALSE(fastDfa.Check("aaaa"));
}

TEST(CHECK_TIMES, CHECK1) {
  std::stringstream in("(a+|b*)");
  auto result = CreateASTFromStream(&in);
  auto nfa = CreateNFAFromAST(result.get());
  auto dfa = CreateDFAFromNFA(nfa);
  auto fastDfa = CreateFastFiniteAutomata(dfa);
  std::string test;
  for (size_t i = 0; i < 100000; ++i) {
    test += "a";
  }
  for (size_t i = 0; i < 100000; ++i) {
    test += "b";
  }

//  Benchmark(
//      [&nfa, &test]() {
//        nfa.Check(test);
//      },
//      "NFA with regex (a+|b*)");
//  Benchmark(
//      [&dfa, &test]() {
//        dfa.Check(test);
//      },
//      "DFA with regex (a+|b*)");
  Benchmark(
      [&fastDfa, &test]() {
        fastDfa.Check(test);
      },
      "Fast DFA with regex (a+|b*)");
}

TEST(CHECK_TIMES, CHECK2) {
  std::stringstream in("((a*|b*|c*)d+e+)|(x*y+z*)");
  auto result = CreateASTFromStream(&in);
  auto nfa = CreateNFAFromAST(result.get());
  auto dfa = CreateDFAFromNFA(nfa);
  auto fastDfa = CreateFastFiniteAutomata(dfa);
  std::string test;
  for (size_t i = 0; i < 100000; ++i) {
    test += "a";
  }
  for (size_t i = 0; i < 100000; ++i) {
    test += "b";
  }

//  Benchmark(
//      [&nfa, &test]() {
//        nfa.Check(test);
//      },
//      "NFA with regex (a+|b*)");
//  Benchmark(
//      [&dfa, &test]() {
//        dfa.Check(test);
//      },
//      "DFA with regex (a+|b*)");
  Benchmark(
      [&fastDfa, &test]() {
        fastDfa.Check(test);
      },
      "Fast DFA with regex (a+|b*)");
}

TEST(CHECK_TIMES, CHECK3) {
  std::stringstream in("((((((((a*)+)+)*)+)*)*)+)+");
  auto result = CreateASTFromStream(&in);
  auto nfa = CreateNFAFromAST(result.get());
  auto dfa = CreateDFAFromNFA(nfa);
  auto fastDfa = CreateFastFiniteAutomata(dfa);
  std::string test;
  for (size_t i = 0; i < 200000; ++i) {
    test += "a";
  }

//  Benchmark(
//      [&nfa, &test]() {
//        nfa.Check(test);
//      },
//      "NFA with regex (a+|b*)");
//  Benchmark(
//      [&dfa, &test]() {
//        dfa.Check(test);
//      },
//      "DFA with regex (a+|b*)");
  Benchmark(
      [&fastDfa, &test]() {
        fastDfa.Check(test);
      },
      "Fast DFA with regex (a+|b*)");
  Benchmark(
      []() {
        std::string test;
        for (size_t i = 0; i < 200000; ++i) {
          test += "a";
        }
      },
      "Simple loop");
}

TEST(CHECK_TIMES, CHECK4) {
  std::stringstream in("((((((((a*)+)+)*)+)*)*)+)+");
  auto result = CreateASTFromStream(&in);
  auto dfa = CreateDFAFromNFA(CreateNFAFromAST(result.get()));
  auto fastDfa = CreateFastFiniteAutomata(dfa);
  std::string test;
  for (size_t i = 0; i < 2000000; ++i) {
    test += "a";
  }

//  Benchmark(
//      [&dfa, &test]() {
//        dfa.Check(test);
//      },
//      "DFA with regex (a+|b*)");
  Benchmark(
      [&fastDfa, &test]() {
        fastDfa.Check(test);
      },
      "Fast DFA with regex (a+|b*)");
  Benchmark(
      []() {
        std::string test;
        for (size_t i = 0; i < 1000000; ++i) {
        }
      },
      "Simple loop");
}

TEST(CHECK_TIMES, FastDFA) {
  std::string testString1, testString2, testString3, testString4;
  for (size_t i = 0; i < 1000000; ++i) {
    testString1 += 'a';
  }
  testString1 += '#';

  for (size_t i = 0; i < 100000; ++i) {
    testString2 += "0123456789";
  }
  testString2 += '#';

  for (size_t i = 0; i < 100000; ++i) {
    testString3 += "acbdef1234";
  }
  testString3 += '#';

  for (size_t i = 0; i < 100000; ++i) {
    testString4 += "-12345.213";
  }

  Benchmark(
      [&testString1]() {
        std::stringstream in("a*");
        auto fastDfa = CreateFastFiniteAutomataFromStream(&in);
        fastDfa.Check(testString1);
      },
      "Fast DFA with regex a*");
  Benchmark(
      [&testString2]() {
        std::stringstream in("\\d+");
        auto fastDfa = CreateFastFiniteAutomataFromStream(&in);
        fastDfa.Check(testString2);
      },
      "Fast DFA with regex \\d+");
  Benchmark(
      [&testString3]() {
        std::stringstream in("[abcdef1-4]+");
        auto fastDfa = CreateFastFiniteAutomataFromStream(&in);
        fastDfa.Check(testString3);
      },
      "Fast DFA with regex [abcdef1-4]+");
  Benchmark(
      [&testString4]() {
        std::stringstream in("(-?\\d+(.\\d+)?)+");
        auto fastDfa = CreateFastFiniteAutomataFromStream(&in);
        fastDfa.Check(testString4);
      },
      "Fast DFA with regex (-?\\d+(.\\d+)?)+");
}

TEST(CHECK_TIMES, StdRegexp) {
  std::string testString1, testString2, testString3, testString4;
  for (size_t i = 0; i < 1000000; ++i) {
    testString1 += 'a';
  }
  testString1 += '#';

  for (size_t i = 0; i < 100000; ++i) {
    testString2 += "0123456789";
  }
  testString2 += '#';

  for (size_t i = 0; i < 100000; ++i) {
    testString3 += "acbdef1234";
  }
  testString3 += '#';

  for (size_t i = 0; i < 100000; ++i) {
    testString4 += "-12345.213";
  }

  Benchmark(
      [&testString1]() {
        std::regex regex("a*");
        std::regex_match(testString1, regex);
      },
      "Std regex with regex a*");
  Benchmark(
      [&testString2]() {
        std::regex regex("\\d+");
        std::regex_match(testString2, regex);
      },
      "Std regex with regex \\d+");
  Benchmark(
      [&testString3]() {
        std::regex regex("[abcdef1-4]+");
        std::regex_match(testString3, regex);
      },
      "Std regex with regex [abcdef1-4]+");
  Benchmark(
      [&testString4]() {
        std::regex regex("(-?\\d+(.\\d+)?)+");
        std::regex_match(testString4, regex);
      },
      "Std regex with regex (-?\\d+(.\\d+)?)+");
}

TEST(BUILD_TIMES, BUILD1) {
  const std::string regex = "(a+|b*)";

  Benchmark(
      [&regex]() {
        std::stringstream in(regex);
        CreateNFAFromStream(&in);
      },
      "NFA from regex " + regex);
  Benchmark(
      [&regex]() {
        std::stringstream in(regex);
        CreateDFAFromStream(&in);
      },
      "DFA from regex " + regex);
  Benchmark(
      [&regex]() {
        std::stringstream in(regex);
        CreateFastFiniteAutomataFromStream(&in);
      },
      "Fast DFA from regex " + regex);
}

TEST(BUILD_TIMES, BUILD2) {
  const std::string regex = "[abcde3-7\\\\]+xy*z+";

  Benchmark(
      [&regex]() {
        std::stringstream in(regex);
        CreateNFAFromStream(&in);
      },
      "NFA from regex " + regex);
  Benchmark(
      [&regex]() {
        std::stringstream in(regex);
        CreateDFAFromStream(&in);
      },
      "DFA from regex " + regex);
  Benchmark(
      [&regex]() {
        std::stringstream in(regex);
        CreateFastFiniteAutomataFromStream(&in);
      },
      "Fast DFA from regex " + regex);
}

TEST(BUILD_TIMES, BUILD3) {
  const std::string regex = "(-?\\d+(.\\d+)?)\\+(-?\\d+(.\\d+)?)=(-?\\d+(.\\d+)?)";

  Benchmark(
      [&regex]() {
        std::stringstream in(regex);
        CreateNFAFromStream(&in);
      },
      "NFA from regex " + regex);
  Benchmark(
      [&regex]() {
        std::stringstream in(regex);
        CreateDFAFromStream(&in);
      },
      "DFA from regex " + regex);
  Benchmark(
      [&regex]() {
        std::stringstream in(regex);
        CreateFastFiniteAutomataFromStream(&in);
      },
      "Fast DFA from regex " + regex);
}

TEST(BUILD_TIMES, BUILD4) {
  const std::string regex = "(-?\\d+(.\\d+)?){,10}";

  Benchmark(
      [&regex]() {
        std::stringstream in(regex);
        CreateNFAFromStream(&in);
      },
      "NFA from regex " + regex);
  Benchmark(
      [&regex]() {
        std::stringstream in(regex);
        CreateDFAFromStream(&in);
      },
      "DFA from regex " + regex);
  Benchmark(
      [&regex]() {
        std::stringstream in(regex);
        CreateFastFiniteAutomataFromStream(&in);
      },
      "Fast DFA from regex " + regex);
}
