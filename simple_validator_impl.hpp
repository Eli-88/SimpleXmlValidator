#pragma once

#include <concepts>
#include <optional>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

#include "define.hpp"
#include "tag.hpp"
#include "tag_validator/tag_validators.hpp"
#include "text_validator/special_character_text_validator.hpp"

// helper to validate tag
auto ValidateTag(const std::string& input,
                 auto start_iterator,
                 TagValidator auto&& tag_validators) {
  auto iterator = start_iterator;

  const auto tag = ExtractTag({iterator, std::end(input)});

  if (std::holds_alternative<InvalidTag>(tag)) {
    return std::make_pair(false, std::end(input));
  }

  // update iterator to current position after extracting tag
  iterator += TagSize(tag);

  const bool result{tag_validators.Process(tag)};

  return std::make_pair(result, iterator);
}

// helper to validate text
auto ValidateText(const std::string& input,
                  auto start_iterator,
                  TextValidator auto&& text_validator) {
  auto iterator = start_iterator;

  auto open_tag_iterator =
      std::find(iterator, std::end(input), kStartTagDelimiter);

  if (!text_validator.Process({iterator, open_tag_iterator})) {
    return std::make_pair(false, std::end(input));
  }

  return std::make_pair(true, std::prev(open_tag_iterator));
}

bool ValidateXml(const std::string& input,
                 TagValidator auto&& tag_validators,
                 TextValidator auto&& text_validators) {
  // first element needs to be '<' else invalid
  if (kStartTagDelimiter != input.front()) {
    return false;
  }

  for (auto iterator = std::begin(input); iterator != std::end(input);) {
    if (kStartTagDelimiter == *iterator) {
      // process tag
      auto [result, last_iterator] =
          ValidateTag(input, std::next(iterator),
                      std::forward<decltype(tag_validators)>(tag_validators));

      if (!result) {
        return false;
      }

      iterator = std::next(last_iterator);
    } else {
      // process text
      auto [result, last_iterator] = ValidateText(
          input, iterator,
          std::forward<decltype(text_validators)>(text_validators));

      if (!result) {
        return false;
      }

      iterator = std::next(last_iterator);
    }
  }

  return tag_validators.Finalize();
}
