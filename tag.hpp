#pragma once

#include <optional>
#include <string_view>
#include <variant>

class BaseTag {
 public:
  explicit BaseTag(std::string_view value) : value_{value} {}

  std::size_t Size() const { return value_.size(); }
  std::string_view Value() const { return value_; }

 protected:
  std::string_view value_;  // contains only the value within, it does not
                            // contain its opening and closing bracket <>
};

class InvalidTag : public BaseTag {
 public:
  explicit InvalidTag(std::string_view value) : BaseTag{value} {}
};

class OpeningTag : public BaseTag {
 public:
  explicit OpeningTag(std::string_view value) : BaseTag{value} {}
};

class ClosingTag : public BaseTag {
 public:
  explicit ClosingTag(std::string_view value) : BaseTag{value} {}
};

class MarkupTag : public BaseTag {
 public:
  explicit MarkupTag(std::string_view value) : BaseTag{value} {}
};

class SelfClosingTag : public BaseTag {
 public:
  explicit SelfClosingTag(std::string_view value) : BaseTag{value} {}
};

class CDataTag : public BaseTag {
 public:
  explicit CDataTag(std::string_view value) : BaseTag{value} {}
};

class XmlDeclarationTag : public BaseTag {
 public:
  explicit XmlDeclarationTag(std::string_view value) : BaseTag{value} {}
};

class ProcessingInstructionTag : public BaseTag {
 public:
  explicit ProcessingInstructionTag(std::string_view value) : BaseTag{value} {}
};

class CommentTag : public BaseTag {
 public:
  explicit CommentTag(std::string_view value) : BaseTag{value} {}
};

using Tag = std::variant<InvalidTag,
                         OpeningTag,
                         ClosingTag,
                         MarkupTag,
                         SelfClosingTag,
                         CDataTag,
                         XmlDeclarationTag,
                         ProcessingInstructionTag,
                         CommentTag>;

// helper function to extract tag
// the tag passed in does not contain the opening delimiter '<', it only
// contains possible closing delimiter i.e. 'Code>' represents '<Code>'
Tag ExtractTag(std::string_view input);

// helper functions
bool MatchOpeningClosingTags(const OpeningTag& opening_tag,
                             const ClosingTag& closing_tag);

// helper function use in ExtractTag
// the input does not contain the opening delimiter '<' hence in this case it
// start with this if valid: ![CDATA[
std::optional<std::string_view> ExtractCDataTag(std::string_view input);

std::optional<std::string_view> ExtractXmlDeclarationTag(
    std::string_view input);

std::optional<std::string_view> ExtactProcessingDeclarationTag(
    std::string_view input);

std::optional<std::string_view> ExtractCommentTag(std::string_view input);

std::size_t TagSize(const Tag& tag);
std::string_view TagValue(const Tag& tag);
