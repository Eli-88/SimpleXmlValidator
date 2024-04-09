#pragma once

#include <concepts>
#include <tuple>

#include "tag.hpp"

template <class T>
concept TagValidator = requires(T t, const Tag& tag) {
  { t.Process(tag) } -> std::same_as<bool>;
  { t.Finalize() } -> std::same_as<bool>;
};

template <TagValidator... Args>
class TagValidators {
 public:
  TagValidators(Args&&... args)
      : validators_{std::make_tuple(std::forward<Args>(args)...)} {}

  bool Process(const Tag& tag) {
    return std::apply(
        [&tag](auto&&... v) {
          bool result{true};
          ((result &= v.Process(tag)), ...);
          return result;
        },
        validators_);
  }

  bool Finalize() {
    return std::apply(
        [](auto&&... v) {
          bool result{true};
          ((result &= v.Finalize()), ...);
          return result;
        },
        validators_);
  }

 private:
  std::tuple<Args...> validators_;
};