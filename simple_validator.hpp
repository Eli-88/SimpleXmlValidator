#pragma once
#include "simple_validator_impl.hpp"
#include "tag_validator/character_tag_validator.hpp"
#include "tag_validator/open_close_tag_validator.hpp"
#include "tag_validator/xml_declaration_tag_validator.hpp"
#include "text_validator/special_character_text_validator.hpp"

inline bool DetermineXml(const std::string& input) {
  auto tag_validators{TagValidators{OpeningClosingTagValidator{},
                                    CharacterTagValidator{},
                                    XmlDeclarationTagValidator{}}};

  auto text_validators{SpecialCharacterTextValidator{}};

  return ValidateXml(input, tag_validators, text_validators);
}
