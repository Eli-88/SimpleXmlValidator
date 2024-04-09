#include "xml_declaration_tag_validator.hpp"

bool XmlDeclarationTagValidator::Process(const Tag& tag) {
  return std::visit(*this, tag);
}

bool XmlDeclarationTagValidator::Finalize() const {
  return true;
}

bool XmlDeclarationTagValidator::operator()(const XmlDeclarationTag& tag) {
  if (other_tag_ or declaration_tag_) {
    return false;
  }

  declaration_tag_ = tag;
  return true;
}
