//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_POETIC_NUMBER_PARSER_NODE_HPP
#define NEDERROCK_SRC_POETIC_NUMBER_PARSER_NODE_HPP

#include "poetic_number_parser_node_pre.hpp"
#include "poetic_digit_separator_parser_node_pre.hpp"
#include "poetic_digits_parser_node_pre.hpp"
#include "poetic_decimal_parser_node_pre.hpp"
#include "token_stream.hpp"
#include "scope_state.hpp"

#include <memory>
#include <istream>

class Poetic_Number_Parser_Node {
public:
  Poetic_Number_Parser_Node(Poetic_Digit_Separator_Parser_Node_Vector&& poetic_digit_separators_1,
                            Poetic_Digits_Parser_Node_Ptr&&             poetic_digits,
                            Poetic_Digit_Separator_Parser_Node_Vector&& poetic_digit_separators_2,
                            Poetic_Decimal_Parser_Node_Ptr&&            poetic_decimal,
                            Poetic_Digit_Separator_Parser_Node_Vector&& poetic_digit_separators_3);

  static Poetic_Number_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
  void generate_cpp(Scope_State_Ptr state) const;
  std::wstring to_string() const;
private:
  Poetic_Digit_Separator_Parser_Node_Vector m_poetic_digit_separators_1;
  Poetic_Digits_Parser_Node_Ptr             m_poetic_digits;
  Poetic_Digit_Separator_Parser_Node_Vector m_poetic_digit_separators_2;
  Poetic_Decimal_Parser_Node_Ptr            m_poetic_decimal;
  Poetic_Digit_Separator_Parser_Node_Vector m_poetic_digit_separators_3;
};

#endif // NEDERROCK_SRC_POETIC_NUMBER_PARSER_NODE_HPP
