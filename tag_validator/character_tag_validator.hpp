#pragma once

#include "tag.hpp"

class CharacterTagValidator {
 public:
  bool Process(const Tag& tag);
  bool Finalize() const;

  bool operator()(auto&&) { return true; }
  bool operator()(const OpeningTag& tag);
  bool operator()(const ClosingTag& tag);
  bool operator()(const SelfClosingTag& tag);

 private:
  static bool ValidateNameFirstCharacter(char16_t c);
  static bool ValidateNameSubsequentCharacter(char16_t c);
};
