#include "tag.hpp"

#include <algorithm>
#include <cstring>

#include "define.hpp"

Tag ExtractTag(std::string_view input) {
  {
    const auto tag = ExtractCommentTag(input);
    if (tag) {
      return CommentTag{*tag};
    }
  }

  {
    const auto tag = ExtractCDataTag(input);
    if (tag) {
      return CDataTag{*tag};
    }
  }

  {
    const auto tag = ExtractXmlDeclarationTag(input);
    if (tag) {
      return XmlDeclarationTag{*tag};
    }
  }

  {
    const auto tag = ExtactProcessingDeclarationTag(input);
    if (tag) {
      return ProcessingInstructionTag{*tag};
    }
  }

  auto delimiter_iter =
      std::find(std::begin(input), std::end(input), kEndTagDelimiter);
  std::string_view tag;

  if (delimiter_iter != std::end(input)) {
    tag = std::string_view{std::begin(input), delimiter_iter};
  }

  // no tag is or empty open close tag (i.e. <></>) is extracted
  if (tag.empty() or (tag.size() == 1 and tag[0] != kTagClosingDelimiter)) {
    return InvalidTag{tag};
  }

  if (kMarkupDelimiter == tag.front()) {
    return MarkupTag{tag};
  }

  if (kTagClosingDelimiter == tag.back()) {
    return SelfClosingTag{tag};
  }

  if (kTagClosingDelimiter == tag.front()) {
    return ClosingTag{tag};
  } else {
    return OpeningTag{tag};
  }
}

std::size_t TagSize(const Tag& tag) {
  return std::visit([](auto&& t) { return t.Size(); }, tag);
}

std::string_view TagValue(const Tag& tag) {
  return std::visit([](auto&& t) { return t.Value(); }, tag);
}

bool MatchOpeningClosingTags(const OpeningTag& opening_tag,
                             const ClosingTag& closing_tag) {
  return std::equal(
      std::begin(opening_tag.Value()), std::end(opening_tag.Value()),
      std::begin(closing_tag.Value()) + 1, std::end(closing_tag.Value()));
}

std::optional<std::string_view> ExtractCDataTag(std::string_view input) {
  // check if tag starts with ![CDATA[
  if (!input.starts_with(kCDataStartCharacters)) {
    return std::nullopt;
  }

  // ensure there is enough characters before searching for ]]>
  if (input.size() - std::strlen(kCDataStartCharacters) <
      std::strlen(kCDataEndCharacters)) {
    return std::nullopt;
  }

  auto cdata_end_iterator =
      std::search(std::begin(input) + std::strlen(kCDataStartCharacters),
                  std::end(input), kCDataEndCharacters,
                  kCDataEndCharacters + std::strlen(kCDataEndCharacters));

  if (cdata_end_iterator == std::end(input)) {
    return std::nullopt;
  }

  // obmit the last character > to match with the rest of the tags
  // i.e ![CDATA[xxxxx]]
  return std::string_view{
      std::begin(input),
      cdata_end_iterator + std::strlen(kCDataEndCharacters) - 1};
}

std::optional<std::string_view> ExtractXmlDeclarationTag(
    std::string_view input) {
  if (!input.starts_with(kXmlDeclarationStart)) {
    return std::nullopt;
  }

  auto xml_end_iterator = std::search(
      std::begin(input) + std::strlen(kXmlDeclarationEnd), std::end(input),
      kXmlDeclarationEnd, kXmlDeclarationEnd + std::strlen(kXmlDeclarationEnd));

  if (xml_end_iterator == std::end(input)) {
    return std::nullopt;
  }

  return std::string_view{
      std::begin(input),
      xml_end_iterator + std::strlen(kXmlDeclarationEnd) - 1};
}

std::optional<std::string_view> ExtactProcessingDeclarationTag(
    std::string_view input) {
  if (!input.starts_with(kProcessingInstructionDelimiter)) {
    return std::nullopt;
  }

  auto pi_end_iterator = std::search(
      std::begin(input) + 1, std::end(input), kProcessingInstructionEnd,
      kProcessingInstructionEnd + std::strlen(kProcessingInstructionEnd));

  if (pi_end_iterator == std::end(input)) {
    return std::nullopt;
  }

  return std::string_view{
      std::begin(input),
      pi_end_iterator + std::strlen(kProcessingInstructionEnd) - 1};
}

std::optional<std::string_view> ExtractCommentTag(std::string_view input) {
  if (!input.starts_with(kCommentStart)) {
    return std::nullopt;
  }

  auto comment_end_iterator = std::search(
      std::begin(input) + std::strlen(kCommentStart), std::end(input),
      kCommentEnd, kCommentEnd + std::strlen(kCommentEnd));

  if (comment_end_iterator == std::end(input)) {
    return std::nullopt;
  }

  return std::string_view{std::begin(input),
                          comment_end_iterator + std::strlen(kCommentEnd) - 1};
}
