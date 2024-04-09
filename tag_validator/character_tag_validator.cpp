#include "character_tag_validator.hpp"

#include <algorithm>
#include <codecvt>
#include <locale>

bool CharacterTagValidator::Process(const Tag& tag) {
  return std::visit(*this, tag);
}

bool CharacterTagValidator::Finalize() const {
  return true;
}

bool CharacterTagValidator::operator()(const OpeningTag& tag) {
  auto convert =
      std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{};

  const std::u16string u16_tag =
      convert.from_bytes(tag.Value().data(), tag.Value().data() + tag.Size());

  if (!ValidateNameFirstCharacter(u16_tag.front())) {
    return false;
  }

  return std::none_of(
      std::begin(u16_tag) + 1, std::end(u16_tag),
      [](char16_t c) { return !ValidateNameSubsequentCharacter(c); });
}

bool CharacterTagValidator::operator()(const ClosingTag& tag) {
  auto convert =
      std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{};

  const std::u16string u16_tag =
      convert.from_bytes(tag.Value().data(), tag.Value().data() + tag.Size());

  if (!ValidateNameFirstCharacter(u16_tag[1])) {
    return false;
  }

  return std::none_of(
      std::begin(u16_tag) + 1, std::end(u16_tag),
      [](char16_t c) { return !ValidateNameSubsequentCharacter(c); });
}

bool CharacterTagValidator::operator()(const SelfClosingTag& tag) {
  auto convert =
      std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{};

  const std::u16string u16_tag =
      convert.from_bytes(tag.Value().data(), tag.Value().data() + tag.Size());

  if (!ValidateNameFirstCharacter(u16_tag.front())) {
    return false;
  }

  return std::none_of(
      std::begin(u16_tag) + 1, std::begin(u16_tag) + u16_tag.size() - 1,
      [](char16_t c) { return !ValidateNameSubsequentCharacter(c); });
}

bool CharacterTagValidator::ValidateNameFirstCharacter(char16_t c) {
  const bool result =
      ((c == U':') || (c >= U'A' && c <= U'Z') || (c == U'_') ||
       (c >= U'a' && c <= U'z') ||
       (c >= U'\u00C0' && c <= U'\u00D6') ||       // [#xC0-#xD6]
       (c >= U'\u00D8' && c <= U'\u00F6') ||       // [#xD8-#xF6]
       (c >= U'\u00F8' && c <= U'\u02FF') ||       // [#xF8-#x2FF]
       (c >= U'\u0370' && c <= U'\u037D') ||       // [#x370-#x37D]
       (c >= U'\u037F' && c <= U'\u1FFF') ||       // [#x37F-#x1FFF]
       (c >= U'\u200C' && c <= U'\u200D') ||       // [#x200C-#x200D]
       (c >= U'\u2070' && c <= U'\u218F') ||       // [#x2070-#x218F]
       (c >= U'\u2C00' && c <= U'\u2FEF') ||       // [#x2C00-#x2FEF]
       (c >= U'\u3001' && c <= U'\uD7FF') ||       // [#x3001-#xD7FF]
       (c >= U'\uF900' && c <= U'\uFDCF') ||       // [#xF900-#xFDCF]
       (c >= U'\uFDF0' && c <= U'\uFFFD') ||       // [#xFDF0-#xFFFD]
       (c >= U'\U00010000' && c <= U'\U0010FFFF')  // [#x10000-#xEFFFF]
      );

  return result;
}

bool CharacterTagValidator::ValidateNameSubsequentCharacter(char16_t c) {
  const auto result =
      (ValidateNameFirstCharacter(c) || (c == U'|') || (c == U'-') ||
       (c == U'.') || (c >= U'0' && c <= U'9') || (c == U'\xB7') ||  // #xB7
       (c >= U'\u0300' && c <= U'\u036F') ||  // [#x0300-#x036F]
       (c >= U'\u203F' && c <= U'\u2040')     // [#x203F-#x2040]
      );
  return result;
}
