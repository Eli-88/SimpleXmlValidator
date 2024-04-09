#include <iostream>
#include <string>
#include <tuple>
#include <vector>

#include "simple_validator.hpp"

using namespace std;

int main(int argc, char* argv[]) {
  // You can use here to test, feel free to modify/add the test cases here.
  // You can also use other ways to test if you want.

  vector<tuple<string, bool>> test_cases = {
      {"<Design><Code>hello world</Code></Design>", true},  // normal case
      {"<Design><Code>hello world</Code></Design><People>",
       false},  // no closing tag for "People"
      {"<People><Design><Code>hello world</People></Code></Design>",
       false},  // "/Code" should come before "/People"
      {"<People age=”1”>hello world</People>",
       false},  // no closing tag for "People age=\"1\"" and no opening tag for
                // "/People"
      {"<Design><Code>hello > world</Code></Design>",
       true},  // > is a valid character within the text section
      {"<Design><Code>hello < world</Code></Design>",
       false},  // < is an invalid character within the text section
      {"invalid <Design><Code>hello > world</Code></Design>",
       false},                      // input must start with >
      {"<>hello world</>", false},  // empty tag is not allowed
      {"l", false},                 // invalid first character
      {"<Design><Code1>hello </Code1>  <Code2> world  <Code3> again </Code3>  "
       "</Code2></Design>",
       true},  // nested tags are valid
      {"<Design><Design>hello world</Design></Design>",
       true},  // duplicated tags are allowed
      {"<Design>hello</Design>  <Code>world</Code>",
       false},  // only 1 parent tag is allowed
      {"< Design><Code>hello world</Code></ Design>",
       false},  // leading whitespace in tag is invalid
      {"<Design><Code>hello</Code> hi <Code>world</Code></Design>",
       true},  // Multiple instances of the same tag are allowed
      {"<Design><Code>hello</Code> <Code1>world</Code1></Design>",
       true},  // Multiple different tags are allowed
      {"<Design><Code>hello</Code> <Code1>world</Code></Design>",
       false},                      // Mismatched closing tag for Code1
      {"<Design></Design>", true},  // Empty tag is valid
      {"<><></>", false},           // Empty tag without a name is invalid
      {"<Design>hello world</Design>",
       true},                  // Text content without nested tags is valid
      {"<Design></>", false},  // Empty tag without content is invalid
      {"<Design><Code>hello world</Design>",
       false},  // Mismatched closing tag for Code
      {"<Design><Code1>hello</Code2></Design>",
       false},                     // Mismatched opening and closing tags
      {"<Design/><Code/>", true},  // Self-closing tags are valid
      {"<Design/Code>",
       false},  // Self-closing tags with incorrect format are invalid
      {"<Design><Code><Text>hello world</Text></Code></Design>",
       true},  // Nested tags with text content are valid
      {"<Design><Code>hello<world</Code></Design>",
       false},  // Invalid character '<' within text content
      {"<Design><Code>hello<</Code></Design>",
       false},  // Invalid character '<<' within text content
      {"<Design><Code>hello ></Code></Design>",
       true},  // Text content with '>' is valid
      {"<Design><Code>hello > world</Code></Design>",
       true},  // Text content with '>' and whitespace is valid
      {"<Design><Code><![CDATA[hello world]]></Code></Design>",
       true},  // CDATA section is valid
      {"<Design><Code><!safsafasfs></Code></Design>",
       true},  // Valid - Declaration within a tag
      {"<Design><!DOCTYPE html><Code>hello world</Code></Design>",
       true},  // Valid - Document type declaration
      {"<Design><!-- Comment --><Code>hello world</Code></Design>",
       true},  // Valid - Comment within a tag
      {"<Design><![CDATA[hello world]]></Design>",
       true},  // Valid - CDATA section
      {"<Design><Code>hello<</Code></Design>",
       false},  // Invalid - Invalid character '<' within text content
      {"<Design><Code>hello ]]> world</Code></Design>",
       true},  // Valid - ']]>' inside CDATA section
      {"<Design><><><></></></></Design>",
       false},  // Invalid - Empty tag without a name
      {"<Design><!DOCTYPE></Design>", true},  // Valid - Empty DOCTYPE
      {"<Design><!DOCTYPE html [<!ELEMENT code (#PCDATA)>]><Code>hello "
       "world</Code></Design>",
       true},  // Valid - DOCTYPE declaration with internal subset
      {"<Design><Code>&#x48;&#x65;&#x6C;&#x6C;&#x6F; world</Code></Design>",
       true},  // Valid - Hexadecimal character references
      {"<Design><Code>&#72;&#101;&#108;&#108;&#111; world</Code></Design>",
       true},  // Valid - Decimal character references
      {"<Design>&lt;Code&gt;hello world&lt;/Code&gt;</Design>",
       true},  // Valid - Escaped angle brackets
      {"<Design><Code>hello < world</Code></Design>",
       false},  // Invalid - '<' is invalid within text content
      {"<Design><Code>hello > world</Code></Design>",
       true},  // Valid - '>' within text content
      {"<Design><Code>hello ' world</Code></Design>",
       true},  // Valid - Single quote within text content
      {"<Design><Code>hello \" world</Code></Design>",
       true},  // Valid - Double quote within text content
      {"<Design><Code>hello \n world</Code></Design>",
       true},  // Valid - Newline within text content
      {"<Design><Code>hello \\ world</Code></Design>",
       true},  // Valid - Backslash within text content
      {"<Text>This is an example of using the greater than sign > within the "
       "text section.</Text>",
       true},  // Greater than sign used directly within text element
      {"<Text>Less than: &lt;, Greater than: &gt;, Ampersand: &amp;, "
       "Quotation: &quot;, Apostrophe: &apos;</Text>",
       true},  // Entity references within text element
      {"<Invalid>& is not allowed here</Invalid>",
       false},  // Invalid use of ampersand outside of entity reference or CDATA
                // section
      {"<Design><Code><![CDATA[hello world]]></Code></Design>",
       true},  // Valid - CDATA section
      {"<Design><Code>&#x48;&#x65;&#x6C;&#x6C;&#x6F; world</Code></Design>",
       true},  // Valid - Hexadecimal character references
      {"<Design><Code>&#72;&#101;&#108;&#108;&#111; world</Code></Design>",
       true},  // Valid - Decimal character references
      {"<Design>&lt;Code&gt;hello world&lt;/Code&gt;</Design>",
       true},  // Valid - Escaped angle brackets
      {"<Design><Code>hello < world</Code></Design>",
       false},  // Invalid - '<' is invalid within text content
      {"<Design><Code>hello > world</Code></Design>",
       true},  // Valid - '>' within text content
      {"<Design><Code>hello ' world</Code></Design>",
       true},  // Valid - Single quote within text content
      {"<Design><Code>hello \" world</Code></Design>",
       true},  // Valid - Double quote within text content
      {"<Design><Code>hello \n world</Code></Design>",
       true},  // Valid - Newline within text content
      {"<Design><Code>hello \\ world</Code></Design>",
       true},  // Valid - Backslash within text content
      {"<Text>This is an example of using the greater than sign > within the "
       "text section.</Text>",
       true},  // Greater than sign used directly within text element
      {"<Text>Less than: &lt;, Greater than: &gt;, Ampersand: &amp;, "
       "Quotation: &quot;, Apostrophe: &apos;</Text>",
       true},  // Entity references within text element
      {"<Invalid>& is not allowed here</Invalid>",
       false},  // Invalid use of ampersand outside of entity reference or CDATA
                // section,
      {"<Design><Code><![CDATA[hello worl<><>d]]></Code></Design>",
       true},  // Valid - Special character used in CDATA

      {"<Design><C-ode>hello world</C-ode></Design>",
       true},  // Valid - '-' is allowed as a subsequent character
      {"<Design><C1ode>hello world</C1ode></Design>",
       true},  // Valid - Digit '1' is allowed as a subsequent  allowed
      {"<Design><C{ode>hello world</C{ode></Design>",
       false},  // Invalid - '{' is not allowed as a subsequent character
      {"<?Design><Code>hello world</Code></?Design>",
       false},  // Invalid - First character cannot be '?'
      {"<_Design><Code>hello world</Code></_Design>",
       true},  // Invalid - First character '_' is allowed
      {"<9Design><Code>hello world</Code></9Design>",
       false},  // Invalid - First character cannot be a digit
      {"<#Design><Code>hello world</Code></#Design>",
       false},                   // Invalid - First character cannot be '#'
      {"<_abc></_abc>", true},   // Valid first character
      {"<.abc></.abc>", false},  // First character is '.', invalid
      {"<A..bc></A..bc>",
       true},  // Valid first character and subsequent characters
      {"<|name></|name>", false},  // First character is '|', invalid
      {"<name|></name|>",
       true},  // Valid first character and subsequent characters
      {"<name-></name->",
       true},  // Valid first character and subsequent characters
      {"<name!></name!>", false},      // subsequent character '!', invalid
      {"<name@></name@>", false},      // subsequent character '@', invalid
      {"<name$></name$>", false},      // subsequent character '$', invalid
      {"<name%></name%>", false},      // subsequent character '%', invalid
      {"<name^></name^>", false},      // subsequent character '^', invalid
      {"<name&></name&>", false},      // subsequent character '&', invalid
      {"<name*></name*>", false},      // subsequent character '*', invalid
      {"<name()></name()>", false},    // subsequent character '(', invalid
      {"<name{}></name{}>", false},    // subsequent character '{', invalid
      {"<name[]></name[]>", false},    // subsequent character '[', invalid
      {"<name/\\></name/\\>", false},  // subsequent character '/', invalid
      {"<nameÄ></nameÄ>", true},       // subsequent character 'Ä', valid
      {"<name日本語></name日本語>",
       true},  // Valid first character and subsequent characters
      {"<nameΔ></nameΔ>", true},  // Valid first character
      {"<name٤></name٤>", true},  // Valid first character
      {"<name₹></name₹>", true},  // Valid first character
      {"<name·></name·>", true},  // Valid first character
      {"<namë></namë>", true},    // Valid first character
      {"<Design>hello</Design> <Design>world</Design>",
       false},               // multiple root with identical tag is Invalid
      {"<1a></1a>", false},  // first character '1' is invalid
      {"<a1></a1>", true},   // first character and subsequent is valid
      {"<?xml version=\"1.0\" encoding=\"UTF-8\"?><root></root>",
       true},  // Valid XML declaration at the beginning
      {"<root><?xml version=\"1.0\" encoding=\"UTF-8\"?><child></child></root>",
       false},  // Invalid XML declaration in the middle
      {"<?xml version=\"1.0\" encoding=\"UTF-8\" ?> <root></root>",
       true},  // Valid XML declaration with whitespace after it
      {"<root></root><?xml version=\"1.0\" encoding=\"UTF-8\"?>",
       false},  // Invalid XML declaration at the end
      {"<root><?xml version=\"1.0\" encoding=\"UTF-8\"?><child></child><?xml "
       "version=\"1.0\" encoding=\"UTF-8\"?></root>",
       false},  // Invalid XML declaration in the middle
  };

  int failed_count = 0;
  for (auto& test_case : test_cases) {
    auto& input = get<0>(test_case);
    auto& expected = get<1>(test_case);

    auto result = DetermineXml(input);
    auto result_str = result ? "Valid" : "Invalid";

    if (result == expected) {
      cout << "OK ";
    } else {
      cout << "NG ";
      failed_count++;
    }
    cout << input << ": " << result_str << endl;
  }

  cout << "Result: " << (test_cases.size() - failed_count) << "/"
       << test_cases.size() << endl;

  return 0;
}
