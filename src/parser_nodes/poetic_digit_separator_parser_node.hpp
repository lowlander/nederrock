//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_POETIC_DIGIT_SEPARATOR_PARSER_NODE_HPP
#define NEDERROCK_SRC_POETIC_DIGIT_SEPARATOR_PARSER_NODE_HPP

#include "poetic_digit_separator_parser_node_pre.hpp"
#include "separator_parser_node_pre.hpp"
#include "token_stream.hpp"

#include <memory>
#include <istream>
#include <variant>
#include <string>

class Poetic_Digit_Separator_Parser_Node {
public:
  struct Choice_1 {
    void dump(std::wostream& output) const;

    Separator_Parser_Node_Ptr m_separator;
  };

  struct Choice_2 {
    void dump(std::wostream& output) const;

    wchar_t m_char;
  };
public:
  Poetic_Digit_Separator_Parser_Node(Choice_1&& choice);
  Poetic_Digit_Separator_Parser_Node(Choice_2&& choice);

  static Poetic_Digit_Separator_Parser_Node_Ptr parse_choice_1(Token_Stream& input);
  static Poetic_Digit_Separator_Parser_Node_Ptr parse_choice_2(Token_Stream& input);
  static Poetic_Digit_Separator_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
private:
  std::variant<Choice_1, Choice_2> m_choice;
};

#endif // NEDERROCK_SRC_POETIC_DIGIT_SEPARATOR_PARSER_NODE_HPP
