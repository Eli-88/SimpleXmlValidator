#pragma once

#include <string>

// delimiters
static constexpr char kStartTagDelimiter = '<';
static constexpr char kEndTagDelimiter = '>';
static constexpr char kTagClosingDelimiter = '/';
static constexpr char kMarkupDelimiter = '!';
static constexpr char kProcessingInstructionDelimiter = '?';

// special character in text section
static const char* kLessThan = "&lt;";
static const char* kGreaterThan = "&gt;";
static const char* kDoubleQuote = "&quot;";
static const char* kApos = "&apos;";
static const char* kAmp = "&amp;";

static const char* kCDataStartCharacters = "![CDATA[";
static const char* kCDataEndCharacters = "]]>";

static const char* kProcessingInstructionEnd = "?>";

static const char* kXmlDeclarationStart = "?xml";
static const char* kXmlDeclarationEnd = kProcessingInstructionEnd;

static const char* kCommentStart = "!--";
static const char* kCommentEnd = "-->";
