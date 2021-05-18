#ifndef REGEX_PARSER_TOKENIZER_HPP
#define REGEX_PARSER_TOKENIZER_HPP

#include <istream>
#include <memory>
#include <utility>
#include <variant>

#include "character_classes.hpp"
#include "util.hpp"

class SymbolToken {
 public:
  SymbolToken() = default;
  explicit SymbolToken(std::vector<char> syms) : syms(std::move(syms)) {}
  bool operator==(const SymbolToken &rhs) const { return syms == rhs.syms; }

 public:
  std::vector<char> syms{};
};

class PlusToken {
 public:
  bool operator==(const PlusToken &) const { return true; }
};

class StarToken {
 public:
  bool operator==(const StarToken &) const { return true; }
};

class QuestionToken {
 public:
  bool operator==(const QuestionToken &) { return true; }
};

class OrToken {
 public:
  bool operator==(const OrToken &) const { return true; }
};

class RangeToken {
 public:
  bool operator==(const RangeToken &rhs) const { return true; }

 public:
  Range range;
};

class Empty {
 public:
  bool operator==(const Empty &) const { return true; }
};

enum class RoundBracketToken { OPEN, CLOSE };

typedef std::variant<SymbolToken, PlusToken, StarToken, OrToken, QuestionToken, RangeToken, RoundBracketToken,
                     Empty>
    Token;

class Tokenizer {
 public:
  explicit Tokenizer(std::istream *in) : stream_(in) { Next(); }

  void Next() {
    if ((stream_->peek() == EOF)) {
      currentToken_ = Empty{};
      return;
    }

    switch (stream_->peek()) {
      case '+': {
        stream_->get();
        currentToken_ = PlusToken{};
        break;
      }
      case '*': {
        stream_->get();
        currentToken_ = StarToken{};
        break;
      }
      case '|': {
        stream_->get();
        currentToken_ = OrToken{};
        break;
      }
      case '?': {
        stream_->get();
        currentToken_ = QuestionToken{};
        break;
      }
      case '(': {
        stream_->get();
        currentToken_ = RoundBracketToken::OPEN;
        break;
      }
      case ')': {
        stream_->get();
        currentToken_ = RoundBracketToken::CLOSE;
        break;
      }
      case '{': {
        stream_->get();
        GetRangeToken();
        break;
      }
      case '[': {
        stream_->get();
        GetCharacterClass();
        break;
      }
      case '\\': {
        stream_->get();
        GetSpecialToken();
        break;
      }
      default: {
        currentToken_ = SymbolToken{{static_cast<char>(stream_->get())}};
        break;
      }
    }
  }

  Token GetToken() const { return currentToken_; }

 private:
  void GetSpecialToken() {
    switch (char c = static_cast<char>(stream_->get()); c) {
      case 'd': {
        currentToken_ = SymbolToken(DigitClass);
        break;
      }
      case 'D': {
        currentToken_ = SymbolToken(NonDigitClass);
        break;
      }
      case 'w': {
        currentToken_ = SymbolToken(WordClass);
        break;
      }
      case 'W': {
        currentToken_ = SymbolToken(NonWordClass);
        break;
      }
      case 's': {
        currentToken_ = SymbolToken(SpaceClass);
        break;
      }
      case 'S': {
        currentToken_ = SymbolToken(NonSpaceClass);
        break;
      }
      default: {
        currentToken_ = SymbolToken{{c}};
      }
    }
  }

  void GetCharacterClass() {
    std::vector<char> syms;
    while ((syms.emplace_back(stream_->get())) != ']') {
      if (syms.back() == '\\') {
        syms.pop_back();
        GetSpecialToken();
        for (const auto &c : std::get<SymbolToken>(currentToken_).syms) {
          syms.emplace_back(c);
        }
      }
      size_t symsSize = syms.size();
      if (symsSize >= 3 && syms[symsSize - 2] == '-') {
        char first = syms[symsSize - 3], last = syms[symsSize - 1];
        syms.erase(begin(syms) + syms.size() - 3, end(syms));
        for (const auto &c : CharRange(first, last)) {
          syms.emplace_back(c);
        }
      }
    }
    syms.pop_back();
    currentToken_ = SymbolToken{syms};
  }

  void GetRangeToken() {
    Range range;
    while (stream_->peek() != ',') {
      assert(isdigit(stream_->peek()));

      range.lowerBound *= 10;
      range.lowerBound += static_cast<size_t>(stream_->get() - '0');
    }
    stream_->get();
    while (stream_->peek() != '}') {
      if (range.upperBound == SIZE_T_MAX) {
        range.upperBound = 0;
      }
      assert(isdigit(stream_->peek()));

      range.upperBound *= 10;
      range.upperBound += static_cast<size_t>(stream_->get() - '0');
    }
    stream_->get();

    assert(range.lowerBound <= range.upperBound);

    currentToken_ = RangeToken{range};
  }

  std::istream *stream_;
  Token currentToken_;
};

#endif  // REGEX_PARSER_TOKENIZER_HPP
