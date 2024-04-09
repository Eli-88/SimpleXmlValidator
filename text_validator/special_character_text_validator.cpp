#include "special_character_text_validator.hpp"

#include <algorithm>
#include <cctype>
#include <vector>

#include "define.hpp"

static const std::vector<std::string_view> kAllSpecialCharacters{
    kAmp, kApos, kDoubleQuote, kGreaterThan, kLessThan};

bool SpecialCharacterTextValidator::Process(std::string_view input) {
  auto iterator = std::begin(input);

  while (iterator != std::end(input)) {
    auto amp_iterator = std::find(iterator, std::end(input), '&');

    if (std::end(input) != amp_iterator) {
      auto semicolon_iterator = std::find(amp_iterator, std::end(input), ';');

      // semicolon is necessary to work with & as a special character
      if (semicolon_iterator == std::end(input)) {
        return false;
      }

      std::string_view extracted_special_character{
          amp_iterator, std::next(semicolon_iterator)};

      bool validated_result{false};

      // check for reserved special characters i.e. &amp;
      validated_result |= (std::end(kAllSpecialCharacters) !=
                           std::find(std::begin(kAllSpecialCharacters),
                                     std::end(kAllSpecialCharacters),
                                     extracted_special_character));

      // check for non hex character reference i.e. &#65;
      if (extracted_special_character.size() > 3 and
          extracted_special_character[1] == '#') {
        validated_result |= std::none_of(
            (std::begin(extracted_special_character) + 3),
            (std::begin(extracted_special_character) +
             extracted_special_character.size() - 1),  // ignore semicolon
            [](const char c) {
              // if none of the character is out of range
              return !(std::isdigit(c));
            });
      }

      // check for hex character reference i.e. &#x65A;
      if (extracted_special_character.size() > 4 and
          extracted_special_character[1] == '#' and
          extracted_special_character[2] == 'x') {
        validated_result |= std::none_of(
            (std::begin(extracted_special_character) + 3),
            (std::begin(extracted_special_character) +
             extracted_special_character.size() - 1),  // ignore semicolon
            [](const char c) {
              // if none of the character is out of range
              return !(std::isdigit(c) or (c >= 'A' and c <= 'F'));
            });
      }

      if (!validated_result) {
        return false;
      }

      iterator = std::next(semicolon_iterator);
    } else {
      iterator = std::next(iterator);
    }
  }

  return true;
}
