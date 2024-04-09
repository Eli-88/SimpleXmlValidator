#pragma once

#include <concepts>
#include <string_view>

template <class T>
concept TextValidator = requires(T validator, std::string_view input) {
  { validator.Process(input) } -> std::same_as<bool>;
};

class SpecialCharacterTextValidator {
 public:
  /**
   * @param input input from the text section only
   * @return true if all special characters are valid
   */
  bool Process(std::string_view input);
};
