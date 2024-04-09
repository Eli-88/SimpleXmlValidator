#pragma once

#include <string_view>
#include <vector>

#include "tag.hpp"

// helper class to keep tracker of opening closing tags pair
class OpeningClosingTagValidator {
 public:
  /**
   * @param tag opening or closing tag, user need to ensure at tag is not empty
   * @return true: when tag is opening tag or closing tag has a corresponding
   * existing opening tag added
   */
  bool Process(const Tag& tag);

  /**
   * it will assume that true when no tag is added at all before calling this
   * method
   * @return true when all opening tag added has a corresponding closing tag
   */
  bool Finalize() const;

  bool operator()(const OpeningTag& tag);
  bool operator()(const ClosingTag& tag);

  // ignore other tags and return true
  bool operator()(auto&&) { return true; }

 private:
  std::vector<OpeningTag> tags_;
  int root_count_{0};
};
