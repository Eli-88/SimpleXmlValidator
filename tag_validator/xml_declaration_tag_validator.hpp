#pragma once

#include <optional>

#include "tag.hpp"

class XmlDeclarationTagValidator {
 public:
  bool Process(const Tag& tag);
  bool Finalize() const;

  bool operator()(const XmlDeclarationTag& tag);
  bool operator()(auto&& tag) {
    other_tag_ = tag;
    return true;
  }

 private:
  std::optional<XmlDeclarationTag> declaration_tag_;
  std::optional<Tag> other_tag_;
};