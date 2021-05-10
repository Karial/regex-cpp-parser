//
// Created by Aleksandr Kazakov on 5/10/21.
//

#ifndef REGEX_PARSER__CHARACTER_CLASSES_HPP_
#define REGEX_PARSER__CHARACTER_CLASSES_HPP_

std::vector<char> ExcludeChars(const std::vector<char> &chars) {
  std::vector<bool> included(256, true);
  size_t included_num = 256;
  for (const auto &c : chars) {
    if (included[c]) {
      included_num--;
    }

    included[c] = false;
  }

  std::vector<char> res(included_num);
  for (size_t i = 0, j = 0; i < 256; ++i) {
    if (included[i]) {
      res[j++] = i;
    }
  }

  return res;
}

std::vector<char> GetWordCharacters() {
  std::vector<char> res = {'_'};

  for (char c = 'a'; c <= 'z'; c++) {
    res.emplace_back(c);
  }

  for (char c = 'A'; c <= 'Z'; c++) {
    res.emplace_back(c);
  }

  for (char c = '0'; c <= '9'; c++) {
    res.emplace_back(c);
  }

  return res;
}

const std::vector<char> DigitClass = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
const std::vector<char> NonDigitClass = ExcludeChars(DigitClass);

const std::vector<char> WordClass = GetWordCharacters();
const std::vector<char> NonWordClass = ExcludeChars(WordClass);

const std::vector<char> SpaceClass = {'\t', '\r', '\n', '\f'};
const std::vector<char> NonSpaceClass = ExcludeChars(SpaceClass);

#endif //REGEX_PARSER__CHARACTER_CLASSES_HPP_
