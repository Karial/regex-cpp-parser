#ifndef REGEX_PARSER_TOKENIZER_HPP
#define REGEX_PARSER_TOKENIZER_HPP

#include <istream>
#include <memory>
#include <utility>
#include <variant>

#include "character_classes.hpp"

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

class Empty {
 public:
  bool operator==(const Empty &) const { return true; }
};

enum class RoundBracketToken { OPEN, CLOSE };

enum class SquareBracketToken { OPEN, CLOSE };

typedef std::variant<SymbolToken, PlusToken, StarToken, OrToken, QuestionToken, RoundBracketToken, SquareBracketToken,
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
      case '[': {
        stream_->get();
        currentToken_ = SquareBracketToken::OPEN;
        break;
      }
      case ']': {
        stream_->get();
        currentToken_ = SquareBracketToken::CLOSE;
        break;
      }
      case '\\': {
        stream_->get();
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
  std::istream *stream_;
  Token currentToken_;
};

#endif  // REGEX_PARSER_TOKENIZER_HPP
