//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_LETTER_PARSER_NODE_HPP
#define NEDERROCK_SRC_LETTER_PARSER_NODE_HPP

#include "letter_parser_node_pre.hpp"
#include "uppercase_letter_parser_node_pre.hpp"
#include "lowercase_letter_parser_node_pre.hpp"
#include "token_stream.hpp"

#include <memory>
#include <istream>
#include <variant>

// uppercase_letter / lowercase_letter

class Letter_Parser_Node {
public:
  struct Choice_1 {
    void dump(std::wostream& output) const;
    wchar_t to_wchar_t() const;

    Uppercase_Letter_Parser_Node_Ptr m_uppercase_letter;
  };

  struct Choice_2 {
    void dump(std::wostream& output) const;
    wchar_t to_wchar_t() const;

    Lowercase_Letter_Parser_Node_Ptr m_lowercase_letter;
  };

public:
  Letter_Parser_Node(Choice_1&& choice);
  Letter_Parser_Node(Choice_2&& choice);

  static Letter_Parser_Node_Ptr parse_choice_1(Token_Stream& input);
  static Letter_Parser_Node_Ptr parse_choice_2(Token_Stream& input);

  static Letter_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;

  wchar_t to_wchar_t() const;
private:
  std::variant<Choice_1, Choice_2> m_choice;
};

#endif // NEDERROCK_SRC_LETTER_PARSER_NODE_HPP
