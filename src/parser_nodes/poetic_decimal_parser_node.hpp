//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_POETIC_DECIMAL_PARSER_NODE_HPP
#define NEDERROCK_SRC_POETIC_DECIMAL_PARSER_NODE_HPP

#include "poetic_decimal_parser_node_pre.hpp"
#include "poetic_decimal_digit_separator_parser_node_pre.hpp"
#include "poetic_decimal_digits_parser_node_pre.hpp"
#include "token_stream.hpp"

#include <memory>
#include <istream>
#include <variant>

class Poetic_Decimal_Parser_Node {
public:
  struct Choice_1 {
    void dump(std::wostream& output) const;
    std::wstring to_string() const;

    wchar_t m_char;
    Poetic_Decimal_Digit_Separator_Parser_Node_Vector m_poetic_decimal_digit_separators_1;
    Poetic_Decimal_Digits_Parser_Node_Ptr m_poetic_decimal_digits;
    Poetic_Decimal_Digit_Separator_Parser_Node_Vector m_poetic_decimal_digit_separators_2;
  };

  struct Choice_2 {
    void dump(std::wostream& output) const;
    std::wstring to_string() const;

    wchar_t m_char;
    Poetic_Decimal_Digit_Separator_Parser_Node_Vector m_poetic_decimal_digit_separators;
  };

public:
  Poetic_Decimal_Parser_Node(Choice_1&& choice);
  Poetic_Decimal_Parser_Node(Choice_2&& choice);

  static Poetic_Decimal_Parser_Node_Ptr parse_choice_1(Token_Stream& input);
  static Poetic_Decimal_Parser_Node_Ptr parse_choice_2(Token_Stream& input);
  static Poetic_Decimal_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
  std::wstring to_string() const;
private:
  std::variant<Choice_1, Choice_2> m_choice;
};

#endif // NEDERROCK_SRC_POETIC_DECIMAL_PARSER_NODE_HPP
