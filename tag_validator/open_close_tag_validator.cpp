#include "open_close_tag_validator.hpp"

#include "define.hpp"

bool OpeningClosingTagValidator::Process(const Tag& tag) {
  return std::visit(*this, tag);
}

bool OpeningClosingTagValidator::operator()(const OpeningTag& tag) {
  tags_.emplace_back(tag);
  return true;
}

bool OpeningClosingTagValidator::operator()(const ClosingTag& tag) {
  if (tags_.empty() || !MatchOpeningClosingTags(tags_.back(), tag)) {
    return false;
  }

  tags_.pop_back();

  root_count_ += tags_.empty() ? 1 : 0;

  return true;
}

bool OpeningClosingTagValidator::Finalize() const {
  return tags_.empty() and (root_count_ < 2);
}
